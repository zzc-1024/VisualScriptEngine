#include "WXAS_ControlUtilities.hpp"

namespace WXAS
{

MouseCaptureHandler::MouseCaptureHandler (wxPanel* panel) :
	panel (panel),
	counter (0)
{

}

void MouseCaptureHandler::OnMouseDown ()
{
	if (counter == 0) {
		panel->CaptureMouse ();
	}
	counter++;
}

void MouseCaptureHandler::OnMouseUp ()
{
	counter--;
	if (counter == 0) {
		panel->ReleaseMouse ();
	}
	if (counter < 0) {
		counter = 0;
	}
}

void MouseCaptureHandler::OnCaptureLost ()
{
	counter--;
}

NUIE::ModifierKeys GetModiferKeysFromEvent (wxKeyboardState& evt)
{
	std::unordered_set<NUIE::ModifierKeyCode> keys;
	if (evt.ControlDown ()) {
		keys.insert (NUIE::ModifierKeyCode::Control);
	}
	if (evt.ShiftDown ()) {
		keys.insert (NUIE::ModifierKeyCode::Shift);
	}
	return NUIE::ModifierKeys (keys);
}

NUIE::Key GetKeyFromEvent (wxKeyEvent& evt)
{
	wxChar unicodeKey = evt.GetUnicodeKey ();
	if (unicodeKey <= WXK_ESCAPE || unicodeKey == WXK_SPACE || unicodeKey == WXK_DELETE || unicodeKey >= WXK_START) {
		int key = evt.GetKeyCode ();
		switch (key) {
			case WXK_ESCAPE:
				return NUIE::Key (NUIE::PressedKeyCode::Escape);
			case WXK_DELETE:
				return NUIE::Key (NUIE::PressedKeyCode::Delete);
		}
	}
	
	NUIE::ModifierKeys modifierKeys = GetModiferKeysFromEvent (evt);
	switch (unicodeKey) {
		case L'C':
			return NUIE::Key (NUIE::PressedKeyCode::Copy);
		case L'V':
			return NUIE::Key (NUIE::PressedKeyCode::Paste);
		case L'Z':
			return NUIE::Key (NUIE::PressedKeyCode::Undo);
		case L'Y':
			return NUIE::Key (NUIE::PressedKeyCode::Redo);
	}

	return NUIE::InvalidKey;
}

NUIE::UICommandPtr SelectCommandFromContextMenu (wxPanel* panel, const NUIE::Point& position, const NUIE::UICommandStructure& commands)
{
	if (commands.IsEmpty ()) {
		return nullptr;
	}

	wxMenu popupMenu;
	wxMenu* currentMenu = &popupMenu;

	size_t currentCommandId = 1000;
	std::unordered_map<size_t, NUIE::UICommandPtr> commandTable;
	std::function<void (const NUIE::UICommandPtr&)> addCommand = [&] (const NUIE::UICommandPtr& command) {
		if (command->HasChildCommands ()) {
			wxMenu* oldMenu = currentMenu;
			currentMenu = new wxMenu ();
			oldMenu->AppendSubMenu (currentMenu, command->GetName ());
			command->EnumerateChildCommands (addCommand);
			currentMenu = oldMenu;
		} else {
			wxMenuItem* currentMenuItem = currentMenu->AppendCheckItem (currentCommandId, command->GetName ());
			currentMenuItem->Check (command->IsChecked ());
			commandTable.insert ({ currentCommandId, command });
			currentCommandId += 1;
		}
	};
	commands.EnumerateCommands (addCommand);

	wxPoint mousePos ((int) position.GetX (), (int) position.GetY ());
	int selectedItem = panel->GetPopupMenuSelectionFromUser (popupMenu, mousePos);
	if (selectedItem == wxID_NONE) {
		return nullptr;
	}

	return commandTable[selectedItem];
}

}