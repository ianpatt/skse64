#include "Translation.h"

#include "common/IFileStream.h"
#include <shlobj.h>
#include <string>
#include "GameStreams.h"
#include "GameSettings.h"
#include "ScaleformState.h"

namespace Translation
{
	void ParseTranslation(BSScaleformTranslator * translator, std::string & name)
	{
		Setting	* setting = GetINISetting("sLanguage:General");
		std::string path = "Interface\\Translations\\";

		// Construct translation filename
		path += name;
		path += "_";
		path += (setting && setting->GetType() == Setting::kType_String) ? setting->data.s : "ENGLISH";
		path += ".txt";

		BSResourceNiBinaryStream fileStream(path.c_str());
		if(!fileStream.IsValid())
			return;
		else
			_MESSAGE("Reading translations from %s...", path.c_str());

		// Check if file is empty, if not check if the BOM is UTF-16
		UInt16	bom = 0;
		UInt32	ret = fileStream.Read(&bom, sizeof(UInt16));
		if(ret == 0) {
			_MESSAGE("Empty translation file.");
			return;
		}
		if(bom != 0xFEFF) {
			_MESSAGE("BOM Error, file must be encoded in UCS-2 LE.");
			return;
		}

		while(true)
		{	
			wchar_t buf[512];

			UInt32	len = fileStream.ReadLine_w(buf, sizeof(buf) / sizeof(buf[0]), '\n');
			if(len == 0) // End of file
				return;

			// at least $ + wchar_t + \t + wchar_t
			if(len < 4 || buf[0] != '$')
				continue;

			wchar_t last = buf[len - 1];
			if(last == '\r')
				len--;

			// null terminate
			buf[len] = 0;

			UInt32 delimIdx = 0;
			for(UInt32 i = 0; i < len; i++)
				if(buf[i] == '\t')
					delimIdx = i;

			// at least $ + wchar_t
			if(delimIdx < 2)
				continue;

			// replace \t by \0
			buf[delimIdx] = 0;

			wchar_t * key = NULL;
			wchar_t * translation = NULL;
			BSScaleformTranslator::GetCachedString(&key, buf, 0);
			BSScaleformTranslator::GetCachedString(&translation, &buf[delimIdx + 1], 0);
			TranslationTableItem item(key, translation);
			translator->translations.Add(&item);
		}
	}

	void ImportTranslationFiles(BSScaleformTranslator * translator)
	{
		char	appdataPath[MAX_PATH];
		ASSERT(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, appdataPath)));

		std::string	modlistPath = appdataPath;
		modlistPath += "\\Skyrim Special Edition\\plugins.txt";

		// Parse mod list file to acquire translation filenames
		IFileStream modlistFile;
		if(modlistFile.Open(modlistPath.c_str()))
		{
			while(!modlistFile.HitEOF())
			{
				char buf[512];
				modlistFile.ReadString(buf, 512, '\n', '\r');

				// skip comments
				if(buf[0] == '#')
					continue;				

				// Determine extension type
				std::string line = buf;

				// SE: added this
				if (line.length() > 0)
				{
					if (line.front() != '*')
						continue; // Skip not enabled files

					line = line.substr(1); // Remove the * from name
				}					

				std::string::size_type lastDelim = line.rfind('.');
				if(lastDelim != std::string::npos)
				{
					std::string ext = line.substr(lastDelim);

					if(_stricmp(ext.c_str(), ".ESM") == 0 || _stricmp(ext.c_str(),".ESP") == 0 || _stricmp(ext.c_str(), ".ESL") == 0)
					{
						std::string name = line.substr(0, lastDelim);
						ParseTranslation(translator, name);
					}
				}
			}
		}

		modlistFile.Close();
	}
}
