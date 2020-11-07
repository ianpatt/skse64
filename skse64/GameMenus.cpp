#include "GameMenus.h"

RelocAddr <_CreateUIMessageData> CreateUIMessageData(0x00325F00);

IMenu::IMenu() :
	view(NULL),
	unk0C(3),
	flags(0),
	unk14(0x12),
	unk18(NULL)
{
}

void IMenu::Render(void)
{
	if (view)
		view->Render();
}

bool MenuManager::IsMenuOpen(BSFixedString * menuName)
{
	return CALL_MEMBER_FN(this, IsMenuOpen)(menuName);
}

GFxMovieView * MenuManager::GetMovieView(BSFixedString * menuName)
{
	IMenu * menu = GetMenu(menuName);
	if (!menu)
		return NULL;

	return menu->view;
}

IMenu * MenuManager::GetMenu(BSFixedString * menuName)
{
	if (!menuName->data)
		return NULL;

	MenuTableItem * item = menuTable.Find(menuName);

	if (!item)
		return NULL;

	IMenu * menu = item->menuInstance;
	if(!menu)
		return NULL;

	return menu;
}

RaceMenuSlider::RaceMenuSlider(UInt32 _filterFlag, const char * _sliderName, const char * _callbackName, UInt32 _sliderId, UInt32 _index, UInt32 _type, UInt8 _unk8, float _min, float _max, float _value, float _interval, UInt32 _unk13)
{
	filterFlag = _filterFlag;
	name = _sliderName;
	strcpy_s(callback, MAX_PATH, _callbackName);
	id = _sliderId;
	index = _index;
	type = _type;
	min = _min;
	max = _max;
	value = _value;
	interval = _interval;
	unk12C = _unk13;
	unk130 = 0x7F7FFFFF;
	unk134 = _unk8;
	pad135[0] = 0;
	pad135[1] = 0;
	pad135[2] = 0;
}

NiPointer<TESObjectREFR> EnemyHealth::GetTarget() const
{
	NiPointer<TESObjectREFR> refr;
	UInt32 refHandle = (*g_thePlayer)->targetHandle;
	LookupREFRByHandle(refHandle, refr);
	if(!refr) {
		refHandle = handle;
		LookupREFRByHandle(refHandle, refr);
	}

	return refr;
}

Notification::Notification(const Notification& other)
{
	CALL_MEMBER_FN(&text, Set)(other.text.Get(), 0);
	CALL_MEMBER_FN(&status, Set)(other.status.Get(), 0);

	CALL_MEMBER_FN(&sound, Set)(other.sound.c_str());
	objectives.CopyFrom(&other.objectives);

	type = other.type;
	quest = other.quest;
	word = other.word;
	time = other.time;
}
