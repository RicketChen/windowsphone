#include "enumport.h"


int portnums = -1;
std::vector<UINT> ports;
std::vector<std::string> friendlyNames;
std::vector<std::string> sPorts;
HRESULT hr = CoInitialize(NULL);

int getport(void)
{
	if (FAILED(hr))
	{
		_tprintf(_T("Failed to initialize COM, Error:%x\n"), hr);
		return -1;
	}

	//Initialize COM security (Required by CEnumerateSerial::UsingWMI)
	hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	if (FAILED(hr))
	{
		_tprintf(_T("Failed to initialize COM security, Error:%x\n"), hr);
		CoUninitialize();
		return -1;
	}
	size_t i = 0;
	UNREFERENCED_PARAMETER(i);
	if (CEnumerateSerial::UsingSetupAPI1(ports, friendlyNames))
	{
		for (i = 0; i < ports.size(); i++)
		{
			//wprintf(L"%s\n", friendlyNames[i].c_str());
			//wprintf(_T("COM%u <%s>\n"), ports[i], friendlyNames[i].c_str());

			/*if (!strcmp((char *)friendlyNames[i].c_str(), "SCI USBModem"))
			{
				printf("com is %d\n", ports[i]);
				return ports[i];
				break;
			}*/
			//cout << "com" << ports[i] << ":" << friendlyNames[i].c_str() << endl;
			if (!strcmp((char *)friendlyNames[i].c_str(), "Prolific USB-to-Serial Comm Port"))
			{
				LOG(INFO) << " com is " << ports[i];
//				printf("com is %d\n", ports[i]);
				return ports[i];
				break;
			}
		}
	}
	CoUninitialize();
	return -1;
}