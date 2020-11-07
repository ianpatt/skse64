#include "skse64/CustomMenu.h"

std::string CustomMenuCreator::swfPath_;

IMenu* CustomMenuCreator::Create(void)
{
	void* p = ScaleformHeap_Allocate(sizeof(CustomMenu));
	if (p)
	{
		IMenu* menu = new (p) CustomMenu(swfPath_.c_str());
		return menu;
	}
	else
	{
		return NULL;
	}
}

void CustomMenuCreator::SetSwfPath(const char* path)
{
	swfPath_ = path;
}

CustomMenu::CustomMenu(const char* swfPath)
{
	CALL_MEMBER_FN(GFxLoader::GetSingleton(), LoadMovie)(this, &view, swfPath, GFxMovieView::ScaleModeType::kNoBorder, 0.0);

	flags = IMenu::kFlag_Modal | IMenu::kFlag_PausesGame;
	unk0C = 0xA;
	unk14 = 1;

	if(!InputEventDispatcher::GetSingleton()->IsGamepadEnabled())
		flags |= IMenu::kFlag_UpdateUsesCursor | IMenu::kFlag_UsesCursor; // Shows the cursor when no gamepad is enabled

#ifdef _CUSTOMMENU_ITEMDISPLAY
	flags |= IMenu::kFlag_RendersOffscreenTargets;
#endif
}
#ifdef _CUSTOMMENU_ITEMDISPLAY
void UpdateItem3D(const FxDelegateArgs & params)
{
	if(params.menu) {
		UInt32 formId = (UInt32)params.args->GetNumber();
		if(formId) {
			TESForm * form = LookupFormByID(formId);
			if(form) {
				CALL_MEMBER_FN(Inventory3DManager::GetSingleton(), UpdateMagic3D)(form, 0);
			}
		} else {
			CALL_MEMBER_FN(Inventory3DManager::GetSingleton(), Clear3D)();
		}
	}
}
#endif
void CustomMenu::Accept(CallbackProcessor * processor)
{
	GString playSound("PlaySound");
	processor->Process(playSound, PlaySoundCallback);

#ifdef _CUSTOMMENU_ITEMDISPLAY
	GString renderItem("UpdateItem3D");
	processor->Process(renderItem, UpdateItem3D);
#endif
}


void CustomMenu::Render()
{
	if(view) {
		view->Render();
#ifdef _CUSTOMMENU_ITEMDISPLAY
		CALL_MEMBER_FN(Inventory3DManager::GetSingleton(), Render)();
#endif
	}
}
