/************************************************************************************

	AstroMenace (Hardcore 3D space shooter with spaceship upgrade possibilities)
	Copyright (c) 2006-2018 Mikhail Kurinnoi, Viewizard


	AstroMenace is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	AstroMenace is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with AstroMenace. If not, see <http://www.gnu.org/licenses/>.


	Web Site: http://www.viewizard.com/
	Project: https://github.com/viewizard/astromenace
	E-mail: viewizard@viewizard.com

*************************************************************************************/

// TODO translate comments
// TODO should be revised in order to use std::string
// TODO entry's children management should use some container (std::list?)

#include "../vfs/vfs.h"
#include "xml.h"

// считаем кол-во строк до текущего положения буфера
#ifdef gamedebug
static unsigned int GetLineNumber(const char *String, unsigned int Pos)
{
	unsigned int LineNumber = 1;
	unsigned int CurrentPos = 0;

	while ((strlen(String) > 0) && (CurrentPos <= Pos)) {
		if (String[0] == '\n') LineNumber++;
		CurrentPos++;
		String++;
	}
	return LineNumber;
}
#else
static unsigned int GetLineNumber(const char *UNUSED(String), unsigned int UNUSED(Pos))
{
	return 0;
}
#endif // gamedebug

//-----------------------------------------------------------------------------
// Включаем в список
//-----------------------------------------------------------------------------
void cXMLDocument::AttachXMLChildEntry(cXMLEntry *ParentXMLEntry, cXMLEntry *ChildXMLEntry)
{
	if ((RootXMLEntry == nullptr) &&
	    (ParentXMLEntry == nullptr)) {
		RootXMLEntry = ChildXMLEntry;
		return;
	}

	if ((ParentXMLEntry == nullptr) ||
	    (ChildXMLEntry == nullptr))
		return;

	// первый в списке...
	if (ParentXMLEntry->LastChild == nullptr) {
		ChildXMLEntry->Prev = nullptr;
		ChildXMLEntry->Next = nullptr;
		ParentXMLEntry->FirstChild = ChildXMLEntry;
		ParentXMLEntry->LastChild = ChildXMLEntry;
	} else { // продолжаем заполнение...
		ChildXMLEntry->Prev = ParentXMLEntry->LastChild;
		ChildXMLEntry->Next = nullptr;
		ParentXMLEntry->LastChild->Next = ChildXMLEntry;
		ParentXMLEntry->LastChild = ChildXMLEntry;
	}
}

//-----------------------------------------------------------------------------
// Исключаем из списка
//-----------------------------------------------------------------------------
void cXMLDocument::DetachXMLChildEntry(cXMLEntry *ParentXMLEntry, cXMLEntry *XMLChildEntry)
{
	if ((ParentXMLEntry == nullptr) ||
	    (XMLChildEntry == nullptr))
		return;

	// переустанавливаем указатели...
	if (ParentXMLEntry->FirstChild == XMLChildEntry)
		ParentXMLEntry->FirstChild = XMLChildEntry->Next;
	if (ParentXMLEntry->LastChild == XMLChildEntry)
		ParentXMLEntry->LastChild = XMLChildEntry->Prev;

	if (XMLChildEntry->Next != nullptr)
		XMLChildEntry->Next->Prev = XMLChildEntry->Prev;
	else if (XMLChildEntry->Prev != nullptr)
		XMLChildEntry->Prev->Next = nullptr;

	if (XMLChildEntry->Prev != nullptr)
		XMLChildEntry->Prev->Next = XMLChildEntry->Next;
	else if (XMLChildEntry->Next != nullptr)
		XMLChildEntry->Next->Prev = nullptr;
}

/*
 *
 */
bool cXMLDocument::ParseTagLine(const char *OriginBuffer, unsigned int StartPosition, const char *Buffer, cXMLEntry *XMLEntry)
{
	// 1 - получаем имя тэга (начинается сразу после символа <, а заканчивается пробелом, >, />, или символом таб)
	auto TagNameEnd = std::string(Buffer).find(" ");
	if ((TagNameEnd == std::string::npos) ||
	    ((std::string(Buffer).find("\t") != std::string::npos) && (TagNameEnd > std::string(Buffer).find("\t"))))
		TagNameEnd = std::string(Buffer).find("\t");
	if ((TagNameEnd == std::string::npos) ||
	    ((std::string(Buffer).find(">") != std::string::npos) && (TagNameEnd > std::string(Buffer).find(">"))))
		TagNameEnd = std::string(Buffer).find(">");
	if ((TagNameEnd == std::string::npos) ||
	    ((std::string(Buffer).find("/>") != std::string::npos) && (TagNameEnd > std::string(Buffer).find("/>"))))
		TagNameEnd = std::string(Buffer).find("/>");

	XMLEntry->Name = std::string(Buffer).substr(1, TagNameEnd - 1);

	// 2 - проверяем наличие атрибутов и заносим их в динамический массив
	unsigned int i = TagNameEnd;
	while (((Buffer + i)[0] != '>') || ((Buffer + i)[0] != '\0')) {
		// пропускаем все пробелы и табы
		while ((((Buffer + i)[0] == ' ') || ((Buffer + i)[0] == '\t')) && ((Buffer + i)[0] != '\0')) i++;
		if ((Buffer + i)[0] == '\0') {
			std::cerr << "XML file corrupted, line: " << GetLineNumber(OriginBuffer, StartPosition) << "\n";
			break;
		}
		// еще раз проверяем, возможно завершение тэга ставили через пробел или таб
		if (((Buffer + i)[0] == '>') || (!strncmp(Buffer + i, "/>", strlen("/>")))) break;

		// находим имя атрибута
		unsigned int AttribNameStart = i;
		while (((Buffer + i)[0] != '=') && ((Buffer + i)[0] != '\0')) i++;
		if ((Buffer + i)[0] == '\0') {
			std::cerr << "XML file corrupted, line: " << GetLineNumber(OriginBuffer, StartPosition) << "\n";
			break;
		}
		unsigned int AttribNameEnd = i;
		// пропускаем все до кавычек (они у нас следующие, после знака равенства)
		i += 2;
		unsigned int AttribDataStart = i;
		while ((((Buffer + i)[0] != '\'') && ((Buffer + i)[0] != '\"')) && ((Buffer + i)[0] != '\0')) i++;
		if ((Buffer + i)[0] == '\0') {
			std::cerr << "XML file corrupted, line: " << GetLineNumber(OriginBuffer, StartPosition) << "\n";
			break;
		}
		unsigned int AttribDataEnd = i;
		i++;

		// собираем новый атрибут и подключаем его к элементу
		std::string tmpAttrIndex = std::string(Buffer).substr(AttribNameStart, AttribNameEnd - AttribNameStart);
		XMLEntry->Attributes[tmpAttrIndex] = std::string(Buffer).substr(AttribDataStart, AttribDataEnd - AttribDataStart);
	}

	// 3 - определяем и номер строки
	XMLEntry->LineNumber = GetLineNumber(OriginBuffer, StartPosition);

	// 4 - определить есть ли в ней атрибут закрытия '/', или у нас есть еще и контент и закрывающий тэг
	if (std::string(Buffer).find("/>") != std::string::npos)
		return false;
	return true;
}

/*
 *
 */
bool cXMLDocument::ParseTagContent(const char *OriginBuffer, unsigned int StartPosition,
				   const char *Buffer, cXMLEntry *ParentXMLEntry)
{
	// проверяем наличие вложенных тэгов
	bool ChildsFound = true;
	auto DetectTagOpenSymbol = std::string(Buffer).find("<");
	// если символа открытия в строке нет - это просто данные, иначе проверяем, что стоит до этого символа
	if (DetectTagOpenSymbol != std::string::npos) {
		unsigned int CurrentPos = 0;
		while(CurrentPos != DetectTagOpenSymbol) {
			// если до открывающего тэга идут не " ", "\t", "\r", "\n", значит у нас просто данные
			if (((Buffer + CurrentPos)[0] != ' ') &&
			    ((Buffer + CurrentPos)[0] != '\t') &&
			    ((Buffer + CurrentPos)[0] != '\r') &&
			    ((Buffer + CurrentPos)[0] != '\n')) {
				ChildsFound = false;
				break;
			}

			CurrentPos++;
		}

	} else ChildsFound = false;

	// 1 - это просто контент, заносим данные и выходи из рекурсии
	if (!ChildsFound) {
		ParentXMLEntry->Content = std::string(Buffer).substr(0, strlen(Buffer));
		return true;
	}
	// 2 - если в строке нашли открывающий символ тэга - идем на рекурсивную обработку строки с хмл данными
	else {
		// в цикле, пока не достигнем конца обрабатываемой строки:
		unsigned int CurrentBufferPosition = 0;
		while(strlen(Buffer) > 0) {
			// находим положение открывающего тэг символа и закрывающего
			DetectTagOpenSymbol = std::string(Buffer).find("<");

			// это может быть комментарий, проверяем
			if (!strncmp(Buffer+DetectTagOpenSymbol, "<!--", strlen("<!--"))) {
				// ищем завершающую часть, и сразу перемещаемся к ней
				auto DetectCommentCloseSymbol = std::string(Buffer).find("-->");
				if (DetectCommentCloseSymbol == std::string::npos) {
					std::cerr << "XML file corrupted, can't find comment end in line "
						  << GetLineNumber(OriginBuffer,
								   StartPosition + DetectTagOpenSymbol + CurrentBufferPosition)
						  << "\n";
					return false;
				}
				Buffer += DetectCommentCloseSymbol + strlen("-->");
				CurrentBufferPosition += DetectCommentCloseSymbol + strlen("-->");
				continue;
			}

			// если в строке уже нет открывающих символов - просто выходим, все проверили
			if (DetectTagOpenSymbol == std::string::npos)
				return true;
			auto DetectTagCloseSymbol = std::string(Buffer).find(">");
			// если был открывающий символ, но нет закрывающего - это ошибка структуры документа
			if (DetectTagCloseSymbol == std::string::npos) {
				std::cerr << "XML file corrupted, can't find element end for element in line "
					  << GetLineNumber(OriginBuffer,
							   StartPosition + DetectTagOpenSymbol + CurrentBufferPosition)
					  << "\n";
				return false;
			}
			DetectTagCloseSymbol += strlen(">");

			// создаем новый элемент и подключаем его к родительскому
			cXMLEntry *XMLEntry = new cXMLEntry;
			AttachXMLChildEntry(ParentXMLEntry, XMLEntry);

			// полученные данные передаем на обработку и анализ строки элемента
			std::string TagString = std::string(Buffer).substr(DetectTagOpenSymbol,
									   DetectTagCloseSymbol - DetectTagOpenSymbol);
			bool ElementHaveContent = ParseTagLine(OriginBuffer,
							       StartPosition + DetectTagOpenSymbol + CurrentBufferPosition,
							       TagString.c_str(), XMLEntry);

			// если у нас закрытый тэг - с этим элементом закончили, идем искать дальше
			if (!ElementHaveContent) {
				Buffer += DetectTagCloseSymbol;
				CurrentBufferPosition += DetectTagCloseSymbol;
				continue;
			}

			// если тэг открытый - ищем завершающий тэг </имя>
			std::string CloseElement{"</" + XMLEntry->Name + ">"};
			auto CloseElementPosition = std::string(Buffer).find(CloseElement);
			// если закрывающего элемента нет - значит файл поврежден
			if (CloseElementPosition == std::string::npos) {
				std::cerr << "XML file corrupted, can't find element end: "
					  << XMLEntry->Name
					  << " in line: "
					  << GetLineNumber(OriginBuffer,
							   StartPosition + DetectTagOpenSymbol + CurrentBufferPosition)
					  << "\n";
				return false;
			}

			// передаем данные на рекурсивную обработку (если закрывающий тэг не стоит сразу после открывающего)
			if (DetectTagCloseSymbol < CloseElementPosition) {
				std::string ElementContent = std::string(Buffer).substr(DetectTagCloseSymbol,
											CloseElementPosition - DetectTagCloseSymbol);
				if (!ParseTagContent(OriginBuffer,
						     DetectTagCloseSymbol + StartPosition + CurrentBufferPosition,
						     ElementContent.c_str(), XMLEntry)) {
					// вернули с ошибкой, выходим
					return false;
				}
			}

			// смещаем буфер
			Buffer += CloseElementPosition + XMLEntry->Name.size() + strlen("</>");
			CurrentBufferPosition += CloseElementPosition + XMLEntry->Name.size() + strlen("</>");
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// Загрузка
//-----------------------------------------------------------------------------
cXMLDocument::cXMLDocument(const char *XMLFileName)
{
	std::cout << "Open XML file: " << XMLFileName << "\n";

	// если что-то было загружено ранее - освобождаем
	this->~cXMLDocument();

	// читаем данные
	std::string Buffer;
	if (vw_VFStoBuffer(XMLFileName, Buffer)) {
		std::cerr << "XML file not found: " << XMLFileName << "\n";
		return;
	}

	// проверяем заголовок
	if (Buffer.find("<?xml") == std::string::npos) {
		std::cerr << "XML file corrupted: " << XMLFileName << "\n";
		return;
	}
	if (Buffer.find("?>") == std::string::npos) {
		std::cerr << "XML file corrupted: " << XMLFileName << "\n";
		return;
	}

	// идем на рекурсивную обработку
	if (!ParseTagContent(Buffer.c_str(), Buffer.find("?>") + strlen("?>"),
			     Buffer.c_str() + Buffer.find("?>") + strlen("?>"), nullptr)) {
		std::cerr << "XML file corrupted: " << XMLFileName << "\n";
		return;
	}

	if (RootXMLEntry == nullptr) {
		std::cerr << "XML file corrupted, root element not found: " << XMLFileName << "\n";
		return;
	}

	return;
}

/*
 * Save XML elements to file recursively.
 */
void cXMLDocument::SaveRecursive(cXMLEntry *XMLEntry, SDL_RWops *File, unsigned int Level)
{
	if (XMLEntry->EntryType == eEntryType::Comment) {
		// comment
		for (unsigned int i = 0; i < Level; i++) {
			SDL_RWwrite(File, "    ", strlen("    "), 1);
		}
		SDL_RWwrite(File, "<!--", strlen("<!--"), 1);
		SDL_RWwrite(File, XMLEntry->Name.data(), XMLEntry->Name.size(), 1);
		SDL_RWwrite(File, "-->\r\n", strlen("-->\r\n"), 1);
	} else {
		// regular element

		// name
		for (unsigned int i = 0; i < Level; i++) {
			SDL_RWwrite(File, "    ", strlen("    "), 1);
		}
		SDL_RWwrite(File, "<", strlen("<"), 1);
		SDL_RWwrite(File, XMLEntry->Name.data(), XMLEntry->Name.size(), 1);

		// attributes
		if (!XMLEntry->Attributes.empty()) {
			for (const auto &TmpAttrib : XMLEntry->Attributes) {
				SDL_RWwrite(File, " ", strlen(" "), 1);
				SDL_RWwrite(File, TmpAttrib.first.data(), TmpAttrib.first.size(), 1);
				SDL_RWwrite(File, "=\"", strlen("=\""), 1);
				SDL_RWwrite(File, TmpAttrib.second.data(), TmpAttrib.second.size(), 1);
				SDL_RWwrite(File, "\"", strlen("\""), 1);
			}
			SDL_RWwrite(File, " ", strlen(" "), 1);
		}

		// data
		if ((XMLEntry->FirstChild != nullptr) || (!XMLEntry->Content.empty())) {
			if (!XMLEntry->Content.empty()) {
				SDL_RWwrite(File, ">", strlen(">"), 1);
				SDL_RWwrite(File, XMLEntry->Content.data(), XMLEntry->Content.size(), 1);
				SDL_RWwrite(File, "</", strlen("</"), 1);
				SDL_RWwrite(File, XMLEntry->Name.data(), XMLEntry->Name.size(), 1);
				SDL_RWwrite(File, ">\r\n", strlen(">\r\n"), 1);
			} else {
				SDL_RWwrite(File, ">\r\n", strlen(">\r\n"), 1);
				cXMLEntry *Tmp = XMLEntry->FirstChild;
				while (Tmp != nullptr) {
					SaveRecursive(Tmp, File, Level + 1);
					Tmp = Tmp->Next;
				}
				for (unsigned int i = 0; i < Level; i++) SDL_RWwrite(File, "    ", strlen("    "), 1);
				SDL_RWwrite(File, "</", strlen("</"), 1);
				SDL_RWwrite(File, XMLEntry->Name.data(), XMLEntry->Name.size(), 1);
				SDL_RWwrite(File, ">\r\n", strlen(">\r\n"), 1);
			}
		} else {
			SDL_RWwrite(File, "/>\r\n", strlen("/>\r\n"), 1);
		}
	}
}

/*
 * Save XML to file (libSDL RWops).
 */
bool cXMLDocument::Save(const std::string &XMLFileName)
{
	std::cout << "Save XML file: " << XMLFileName << "\n";

	SDL_RWops *File = SDL_RWFromFile(XMLFileName.c_str(), "wb");
	if (File == nullptr) {
		std::cerr << "Can't open XML file for write " << XMLFileName << "\n";
		return false;
	}

	// XML header
	const std::string tmpXMLHeader{"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\r\n"};
	SDL_RWwrite(File, tmpXMLHeader.data(), tmpXMLHeader.size(), 1);

	if (RootXMLEntry == nullptr) {
		SDL_RWclose(File);
		return true;
	}

	// save all data recursively
	SaveRecursive(RootXMLEntry, File, 0);

	SDL_RWclose(File);
	return true;
}
