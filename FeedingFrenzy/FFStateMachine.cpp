#include "FFStateMachine.h"
#include <algorithm>

bool FFStateMachine::FFStateMachineNodeTransWay::Check() //Check if Transportable
{
	return OnCheckTransportDelegate.GetStdFunction()(SourceStateMachineNode, TargetStateMachineNode);
}

FFStateMachine::FFStateMachineNodeTransWay::FFStateMachineNodeTransWay(class FFStateMachineNode* SourceStateMachineNode, class FFStateMachineNode* TargetStateMachineNode, OnCheckDelegateType OnCheckDelegate)
{
	this->SourceStateMachineNode = SourceStateMachineNode;
	this->TargetStateMachineNode = TargetStateMachineNode;
	this->OnCheckTransportDelegate = OnCheckDelegate;
}

void FFStateMachine::FFStateMachineNodeTransWay::SetOnCheckTransportDelegate(OnCheckDelegateType OnCheckDelegate)
{
	OnCheckTransportDelegate = OnCheckDelegate;
}

void FFStateMachine::FFStateMachineNodeTransWay::SetPriority(int TargetPriority)
{
	Priority = TargetPriority;
}

int FFStateMachine::FFStateMachineNodeTransWay::GetPriority()
{
	return Priority;
}

FFStateMachine::FFStateMachineNode* FFStateMachine::FFStateMachineNodeTransWay::GetSourceNode()
{
	return SourceStateMachineNode;
}

FFStateMachine::FFStateMachineNode* FFStateMachine::FFStateMachineNodeTransWay::GetTargetNode()
{
	return TargetStateMachineNode;
}

FFStateMachine::FFStateMachineNode::FFStateMachineNode(std::string StateName)
{
	this->StateName = StateName;
}

std::string FFStateMachine::FFStateMachineNode::GetName()
{
	return StateName;
}

void FFStateMachine::FFStateMachineNode::SetOnNodeEntered(OnNodeEvent NodeDelegate)
{
	OnNodeEntered = NodeDelegate;
}

void FFStateMachine::FFStateMachineNode::SetOnNodeLeaved(OnNodeEvent NodeDelegate)
{
	OnNodeLeaved = NodeDelegate;
}

void FFStateMachine::FFStateMachineNode::AddTransportWay(FFStateMachineNode* TargetNode, OnCheckDelegateType OnCheckDelegate)
{
	TransWayCollection.push_back(new FFStateMachineNodeTransWay(this, TargetNode, OnCheckDelegate));
}

FFStateMachine::FFStateMachineNode* FFStateMachine::FFStateMachineNode::Check()
{
	std::vector<FFStateMachineNodeTransWay*> TransportableWay;
	for (auto it = TransWayCollection.begin(); it < TransWayCollection.end(); ++it)
	{
		if ((*it)->Check())
		{
			TransportableWay.push_back(*it);
		}
	}
	if (!TransportableWay.empty())
		return this;
	std::sort(TransportableWay.begin(), TransportableWay.end(), [](FFStateMachineNodeTransWay* a, FFStateMachineNodeTransWay* b)
			  {
				  return a->GetPriority() > b->GetPriority();
			  });
	TransportableWay[0]->GetSourceNode()->OnNodeLeaved.GetStdFunction()(TransportableWay[0]);
	TransportableWay[0]->GetTargetNode()->OnNodeEntered.GetStdFunction()(TransportableWay[0]);
	return TransportableWay[0]->GetTargetNode();
}

void FFStateMachine::FFStateMachineNode::BreakTransportWayTo(FFStateMachineNode* TargetStateNode)
{
	auto it = std::find_if(TransWayCollection.begin(), TransWayCollection.end(), [=](FFStateMachineNodeTransWay* TransWay)
						   {
							   return TransWay->GetTargetNode() == TargetStateNode;
						   });
	if (it != TransWayCollection.end())
	{
		delete *it;
		TransWayCollection.erase(it);
	}
}

void FFStateMachine::FFStateMachineNode::CheckInvalidConnection(void* InvalidNode)
{
	for (auto it = TransWayCollection.begin(); it < TransWayCollection.end(); ++it)
	{
		if ((*it)->GetTargetNode() == InvalidNode)
		{
			it = TransWayCollection.erase(it);
		}
	}
}

FFStateMachine::FFStateMachineNode::~FFStateMachineNode()
{
	for (auto it = TransWayCollection.begin(); it < TransWayCollection.end(); ++it)
	{
		delete *it;
		it = TransWayCollection.erase(it);
	}
}

FFStateMachine::FFStateMachine()
{
	TimerHandle = FFTimerManager::Get().SetTimerPerTick(FFDelegate<void(float)>::CreateFromLambda([this](float DetailTime)
																								  {
																									  if (!CurrentState)
																										  return;
																									  CurrentState = CurrentState->Check();
																									  CurrentState->OnNodeStaying.GetStdFunction()(CurrentState);
																								  }));
}

void FFStateMachine::AddState(FFStateMachineNode* NewState)
{
	if (std::find(NodeCollection.begin(), NodeCollection.end(), NewState) == NodeCollection.end())
		return;
	NodeCollection.push_back(NewState);
}

FFStateMachine::FFStateMachine::FFStateMachineNode* FFStateMachine::FindStateByName(std::string StateName)
{
	auto it = std::find_if(NodeCollection.begin(), NodeCollection.end(), [=](FFStateMachineNode* A)->bool
						   {
							   return A->GetName() == StateName;
						   });
	if (it != NodeCollection.end())
		return *it;
	else
		return nullptr;
}

bool FFStateMachine::RemoveStateByName(std::string StateName)
{
	auto it = std::find_if(NodeCollection.begin(), NodeCollection.end(), [=](FFStateMachineNode* A)->bool
						   {
							   return A->GetName() == StateName;
						   });
	if (it != NodeCollection.end())
	{
		delete *it;
		NodeCollection.erase(it);
		return true;
	}
	else
		return false;
}

FFStateMachine::~FFStateMachine()
{
	for (auto it = NodeCollection.begin(); it < NodeCollection.end(); ++it)
	{
		delete *it;
		FFTimerManager::Get().CancelTimer(TimerHandle);
	}
}
