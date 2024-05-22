#pragma once
#include <libds/amt/hierarchy.h>
#include <libds/amt/explicit_hierarchy.h>
#include <libds/heap_monitor.h>
#include <Windows.h>
#include "NacitavacUJ.h"
#include "TriediaciAlgoritmus.h"
#include "AlgoritmusNaPredikat.h"
#include "TabulkaUJ.h"
#include <crtdbg.h>
#include <libds/amt/implicit_sequence.h>
class Program
{
public:
	void loop()
	{
		{
			//ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*> CR;
			//CR.data_ = new UzemnaJednotka("CR","0" ,TypUJ::republika);
			//ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*> hierarchy;
			//hierarchy.emplaceRoot();
			//hierarchy.changeRoot(&CR);
			//ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka> CR;
			//hierarchy.changeRoot(&CR);
			SetConsoleOutputCP(65001);
			SetConsoleCP(65001);//
			//std::locale::global(std::locale(""));
			NacitavacUJ nc;
			std::vector<UzemnaJednotka*> kraje;
			std::vector<UzemnaJednotka*> soorp;
			std::vector<Obec*> obce;

			ds::amt::ImplicitSequence<UzemnaJednotka*> triediacaSeq;
			
			
			ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>* hierarchy = new ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>;
			nc.nacitaj("CR.txt", hierarchy, kraje, soorp, obce);
			nc.nacitajTabulky(kraje, soorp, obce);
			//nc.nacitajHierarchiu(hierarchy, kraje, soorp, obce);
			//nc.nacitajHierarchiu()
			AlgoritmusNaPredikat al;
			const auto& startsWithStr = [](UzemnaJednotka* uj, std::string startingString) {
				std::string word = uj->getNazovUzemnejJednotky();
				if (startingString.length() > word.length()) {
					return false;
				}
				for (size_t i = 0; i < startingString.length(); i++) {
					if (i < word.length())
					{
						if (startingString[i] != word[i]) {
							return false;
						}
					}
				}
				return true;
				};
			//lambda na filtrovanie
			const auto& containsStr = [](UzemnaJednotka* uj, std::string containingString) {
				int lengthOfStr = 0;
				std::string word = uj->getNazovUzemnejJednotky();
				for (size_t i = 0; i < word.length() + 1; i++)
				{

					if (lengthOfStr >= containingString.length()) {
						return true;
					}
					if (i < word.length()) {
						if (word[i] == containingString[lengthOfStr]) {
							lengthOfStr++;
						}
						else {
							lengthOfStr = 0;
						}
					}


				}
				return false;

				};
			
			const auto hasType = [](UzemnaJednotka* uj, std::string _typ) {
				return uj->getTypString() == _typ;
				};

			const auto& compareAlphabetical = [](const UzemnaJednotka& prva, const UzemnaJednotka& druha)->bool
			{
				int cmp = strcmp(prva.getNazovUzemnejJednotky().c_str(), druha.getNazovUzemnejJednotky().c_str());
				if(cmp < 0)
				{
					return true;
				}
				else
				{
					return false;
				}
			};

			const auto& compareConsonantCount = [](const UzemnaJednotka& prva, const UzemnaJednotka& druha)->bool
				{
					std::string consonants = "bcčdďfghjklĺľmnňpqrřsštťvwxzž";
					int pocetPrvej = 0;
					int pocetDruhej = 0;
					for (size_t i = 0; i < prva.getNazovUzemnejJednotky().length(); i++)
					{
						if (consonants.find(prva.getNazovUzemnejJednotky()[i]) != std::string::npos)
						{
							++pocetPrvej;
						}
					}
					for (size_t i = 0; i < druha.getNazovUzemnejJednotky().length(); i++)
					{
						if (consonants.find(druha.getNazovUzemnejJednotky()[i]) != std::string::npos)
						{
							++pocetDruhej;
						}
					}
					if (pocetPrvej < pocetDruhej)
					{
						return true;
					}
					else
					{
						return false;
					}
				};
			TriediaciAlgoritmus ta;
			
			//al.vypis(al.najdiAk(kraje, "H", containsStr));
			//al.vypis(soorp);
			//al.vypis(obce);
			bool vybrane = false;
			std::string uzemjednotka;
			std::vector<UzemnaJednotka*> temp;
			//std::cout << "Vektory alebo hierarchia?";
			std::string vstup;
			std::cin >> vstup;
			if (vstup == "vektory") {
				/*
				while (!vybrane)
				{
					std::cout << "Vyberte typ uzemnej jednotky: {kraj, soorp, obec}" << std::endl;
					std::cin >> uzemjednotka;
					if (uzemjednotka == "kraj")
					{
						temp = kraje;
						vybrane = true;
					}
					else if (uzemjednotka == "soorp")
					{
						temp = soorp;
						vybrane = true;
					}
					else if (uzemjednotka == "obec")
					{
						temp = obce;
						vybrane = true;
					}
					else {
						std::cout << "Zle zadany nazov uzemnej jednotky" << std::endl;
					}
				}
				vybrane = false;
				while (!vybrane)
				{
					std::cout << "Chcete filtrovat uzemne jednotky? {ano, nie}" << std::endl;
					std::string rozhodnutie;
					std::cin >> rozhodnutie;
					if (rozhodnutie == "ano")
					{
						std::string predString;
						std::cout << "Napiste string podla ktoreho chcete uzemne jednotky filtrovat:" << std::endl;
						std::cin >> predString;
						std::cout << "Akym sposobom chcete uzemne jednotky filtrovat? {startsWithStr, containsStr}" << std::endl;
						std::string sposob;
						std::cin >> sposob;
						if (sposob == "startsWithStr")
						{
							temp = al.najdiAk<UzemnaJednotka>(temp.begin(), temp.end(), predString, startsWithStr);

						}
						if (sposob == "containsStr")
						{
							temp = al.najdiAk<UzemnaJednotka>(temp.begin(), temp.end(), predString, containsStr);
						}
						al.vypis(temp.begin(), temp.end());
					}
					else {
						vybrane = true;
					}


				}
				al.vypis(temp.begin(), temp.end());
				kraje.clear();
				soorp.clear();
				obce.clear();
				hierarchy->clear();
				*/
			}
			else if (vstup == "hierarchia")
			{
				bool end = false;
				auto node = hierarchy->accessRoot();
				auto node2 = hierarchy->accessSon(*node, 0);
				auto typSyna = hierarchy->accessSon(*node, 0)->data_->getTyp();
				//std::cout << node->data_->getNazovUzemnejJednotky();
				ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>>::PreOrderHierarchyIterator*
					iterator = new ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>>::PreOrderHierarchyIterator(hierarchy, node);
				iterator->operator*()->getNazovUzemnejJednotky();
				int indicator = 1;
				while (end == false)
				{
					std::cout << "Chcete skoncit?" << std::endl;
					std::string koniec;
					std::cin >> koniec;
					if (koniec == "ano")
					{
						end = true;
						delete iterator;
						break;
					}
					while (iterator->operator!=(hierarchy->end()))
					{

						bool found = false;
						if (iterator->operator*()->getTyp() == typSyna)
						{
							std::cout << iterator->operator*()->getNazovUzemnejJednotky() << " " << indicator << "." << std::endl;
							indicator++;
						}
						iterator->operator++();
					}


					std::cout << "Chcete filtrovat? {ano, nie}" << std::endl;
					std::string rozhodnutie;
					std::cin >> rozhodnutie;
					if (rozhodnutie == "ano")
					{
						std::string predString;
						std::cout << "Napiste string podla ktoreho chcete uzemne jednotky filtrovat:" << std::endl;
						std::cin >> predString;
						std::string filter;
						std::cout << "Akym sposobom chcete uzemne jednotky filtrovat? {startsWithStr, containsStr, hasType {kraj, soorp, obec}}" << std::endl;
						std::cin >> filter;
						delete iterator;
						iterator = new ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>>::PreOrderHierarchyIterator(hierarchy, node);
						if (filter == "startsWithStr")
						{
							temp = al.najdiAk<UzemnaJednotka>(*iterator, hierarchy->end(), predString, startsWithStr);
						}
						if (filter == "containsStr")
						{
							temp = al.najdiAk<UzemnaJednotka>(*iterator, hierarchy->end(), predString, containsStr);
						}
						if (filter == "hasType")
						{
							temp = al.najdiAk<UzemnaJednotka>(*iterator, hierarchy->end(), predString, hasType);
						}
						al.vypis(temp.begin(), temp.end());
						
					}
					indicator = 1;


					std::cout << "Ktoreho syna chcete zvolit? (stlacenim 0 zvolite predka)" << std::endl;
					size_t zvolenySyn;
					std::cin >> zvolenySyn;
					if (zvolenySyn == 0)
					{
						if (hierarchy->accessParent(*node) != nullptr)
						{
							node = hierarchy->accessParent(*node);
							typSyna = hierarchy->accessSon(*node, 0)->data_->getTyp();
						}
						else
						{
							std::cout << "Nemate ziadneho predka" << std::endl;
						}

					}
					else if (hierarchy->accessSon(*node, zvolenySyn - 1) != nullptr)
					{


						node = hierarchy->accessSon(*node, zvolenySyn - 1);
						if (node->data_->getTyp() != TypUJ::obec)
						{
							typSyna = hierarchy->accessSon(*node, 0)->data_->getTyp();
						}
						else
						{
							node = hierarchy->accessParent(*node);
							std::cout << "Nemate ziadneho syna" << std::endl;
							typSyna = TypUJ::obec;
						}
						//typSyna = hierarchy->accessSon(*node, 0)->data_->getTyp();





						//std::cout << "Nemate ziadneho syna" << std::endl;

					}
					else
					{
						std::cout << "Zly vyber" << std::endl;
					}
					

					delete iterator;
					iterator = new ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>>::PreOrderHierarchyIterator(hierarchy, node);
					indicator = 1;
				}
				/*
				for (auto uzemna_jednotka : temp)
				{
					delete uzemna_jednotka;
				}
				hierarchy->clear();
				*/
				//delete iterator;

			}
			
			else if (vstup == "tabulky")
			{
				std::cout << "S akou tabulkou chcete pracovat? {kraje, soorp, obce}" << std::endl;
				std::string typTabulky;
				std::cin >> typTabulky;
				std::cout << "Zadajte nazov uzemnej jednotky ktoru chcete vyhladat" << std::endl;
				std::string nazovUJ;
				std::cin.ignore();
				std::getline(std::cin, nazovUJ);
				//std::cout << nc.getTabulkaObce()->size() << std::endl;
				switch (std::tolower(typTabulky[0]))
				{
				case 'k':
					nc.vypisTabulkuPodlaKluca(nazovUJ, nc.getTabulkaKraje());
					break;
					case 's':
						nc.vypisTabulkuPodlaKluca(nazovUJ, nc.getTabulkaSoorp());
					break;
					case 'o':
						nc.vypisTabulkuPodlaKluca(nazovUJ, nc.getTabulkaObce());
					break;
				}
			}

			
			//delete[] hierarchy;;
			
			for (auto uzemna_jednotka : kraje)
			{
				delete uzemna_jednotka;
			}
			for (auto uzemna_jednotka : soorp)
			{
				delete uzemna_jednotka;
			}
			for (auto uzemna_jednotka : obce)
			{
				delete uzemna_jednotka;
			}
			
			delete hierarchy->accessRoot()->data_;
			delete hierarchy;
			
		}
	}
};

