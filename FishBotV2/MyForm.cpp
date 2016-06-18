#include "MyForm.h"

using namespace System;
using namespace FishBotV2;

int main(array <String^>^args)
{

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew MyForm());
	return 0;
}