#pragma once
#include "FFDelegate.h"
#include "FFTimerManager.h"

class FFStateMachine
{
	class FFStateMachineNode;
	typedef FFDelegate<bool(class FFStateMachineNode*  /*SourceNode*/, class FFStateMachineNode*  /*TargetNode*/)> OnCheckDelegateType;
	class FFStateMachineNodeTransWay
	{
		friend FFStateMachine::FFStateMachineNode;
		FFStateMachineNode * SourceStateMachineNode;
		FFStateMachineNode* TargetStateMachineNode;
		FFStateMachine* AttachedStateMachine;
		OnCheckDelegateType OnCheckTransportDelegate;
		int Priority = 0;
	protected:
		bool Check();	//Check if Transportable
	public:
		FFStateMachineNodeTransWay(class FFStateMachineNode* SourceStateMachineNode, class FFStateMachineNode* TargetStateMachineNode, OnCheckDelegateType OnCheckDelegate);
		void SetOnCheckTransportDelegate(OnCheckDelegateType OnCheckDelegate);
		void SetPriority(int TargetPriority);
		int GetPriority();
		FFStateMachine::FFStateMachineNode* GetSourceNode();
		FFStateMachine::FFStateMachineNode* GetTargetNode();
	};
	class  FFStateMachineNode
	{
	public:
		friend FFStateMachine;
		friend FFStateMachineNode;
		typedef FFDelegate<void(class FFStateMachineNodeTransWay*)> OnNodeEvent;
	private:
		std::vector<class FFStateMachineNodeTransWay*> TransWayCollection;
		OnNodeEvent OnNodeEntered;
		FFDelegate<void(FFStateMachineNode*)> OnNodeStaying;
		OnNodeEvent OnNodeLeaved;
		std::string StateName;
	public:
		FFStateMachineNode(std::string StateName);
		std::string GetName();
		void SetOnNodeEntered(OnNodeEvent NodeDelegate);
		void SetOnNodeLeaved(OnNodeEvent NodeDelegate);
		void AddTransportWay(FFStateMachineNode* TargetNode, OnCheckDelegateType OnCheckDelegate);
		FFStateMachineNode* Check();
		void BreakTransportWayTo(FFStateMachineNode* TargetStateNode);
		void CheckInvalidConnection(void* InvalidNode);
		~FFStateMachineNode();
	};
	std::vector<FFStateMachineNode*> NodeCollection;
	FFStateMachineNode* CurrentState;
	FFTimerHandle TimerHandle;
public:
	FFStateMachine();
	void AddState(FFStateMachineNode* NewState);
	FFStateMachineNode* FindStateByName(std::string StateName);
	bool RemoveStateByName(std::string StateName);
	~FFStateMachine();
};