#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Command.h"
#include "Message.h"
#include "File.h"
#include "Items.h"
#include "Interface/InterfacePanel.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool EqualsRequestString(Task *task1, Task *task2)
{
    Message *msg1 = task1->GetMessage();
    Message *msg2 = task2->GetMessage();

    return (Command::FDrive_RequestFileString == msg1->TakeByte() == msg2->TakeByte()) &&
        (msg1->TakeWord() == msg2->TakeWord());
}

void FileRequests::SendRequestForString(String *name, int numString)
{
    Message *message = new Message(Command::FDrive_RequestFileString, (uint)numString, name->CString());

    Task *task = new Task(message, Items::Handler::Processing, EqualsRequestString);

    Interface::AddTask(task);
}
