#include "PapyrusFormList.h"
#include "PapyrusArgs.h"

#include "GameForms.h"

class FormListArray : public BGSListForm::Visitor
{
public:
	FormListArray(VMResultArray<TESForm*> * result) : m_result(result) { }

	virtual bool Accept(TESForm * form)
	{
		m_result->push_back(form);
		return false;
	}

private:
	VMResultArray<TESForm*> * m_result;
};

namespace papyrusFormList
{
	VMResultArray<TESForm*> ToArray(BGSListForm * list)
	{
		VMResultArray<TESForm*> result;
		if(!list)
			return result;

		FormListArray visitor(&result);
		list->Visit(visitor);
		return result;
	}

	void AddForms(BGSListForm * list, VMArray<TESForm*> forms)
	{
		if(list)
		{
			for(UInt32 i = 0; i < forms.Length(); i++)
			{
				TESForm * form = NULL;
				forms.Get(&form, i);
				if(form)
					CALL_MEMBER_FN(list, AddFormToList)(form);
			}
		}
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusFormList::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0 <BGSListForm, VMResultArray<TESForm*>> ("ToArray", "FormList", papyrusFormList::ToArray, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSListForm, void, VMArray<TESForm*>> ("AddForms", "FormList", papyrusFormList::AddForms, registry));

	registry->SetFunctionFlags("FormList", "ToArray", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("FormList", "AddForms", VMClassRegistry::kFunctionFlag_NoWait);
}
