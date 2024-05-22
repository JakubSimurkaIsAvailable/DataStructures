#pragma once
#include <fstream>
#include <vector>
#include <libds/amt/hierarchy.h>
#include <libds/amt/explicit_hierarchy.h>
#include <libds/heap_monitor.h>
#include <libds/adt/list.h>
#include <iostream>
#include <sstream>
#include "UzemnaJednotka.h"
#include "libds/adt/table.h"

class NacitavacUJ
{
private:
	const std::string rozdelovac = ";";
	std::vector<UzemnaJednotka*> kraje_;
	std::vector<UzemnaJednotka*> soorp_;
	std::vector<Obec*> obce_;
	ds::adt::GeneralBinarySearchTree<std::string, ds::adt::ImplicitList<UzemnaJednotka*>*, ds::adt::TableItem<std::string, ds::adt::ImplicitList<UzemnaJednotka*>*>>* tabulkaKraje;
	ds::adt::GeneralBinarySearchTree<std::string, ds::adt::ImplicitList<UzemnaJednotka*>*, ds::adt::TableItem<std::string, ds::adt::ImplicitList<UzemnaJednotka*>*>>* tabulkaSoorp;
	ds::adt::GeneralBinarySearchTree<std::string, ds::adt::ImplicitList<Obec*>*, ds::adt::TableItem<std::string, ds::adt::ImplicitList<Obec*>*>>* tabulkaObce;
public:
		
		
		
	NacitavacUJ()
	{
		tabulkaKraje = new ds::adt::GeneralBinarySearchTree<std::string, ds::adt::ImplicitList<UzemnaJednotka*>*, ds::adt::TableItem<std::string, ds::adt::ImplicitList<UzemnaJednotka*>*>>();
		tabulkaSoorp = new ds::adt::GeneralBinarySearchTree<std::string, ds::adt::ImplicitList<UzemnaJednotka*>*, ds::adt::TableItem<std::string, ds::adt::ImplicitList<UzemnaJednotka*>*>>();
		tabulkaObce = new ds::adt::GeneralBinarySearchTree<std::string, ds::adt::ImplicitList<Obec*>*, ds::adt::TableItem<std::string, ds::adt::ImplicitList<Obec*>*>>();
	}

	~NacitavacUJ()
	{
		for (auto kraj : kraje_)
		{
			delete kraj;
		}
		for (auto soorp : soorp_)
		{
			delete soorp;
		}
		for (auto obec : obce_)
		{
			delete obec;
		}
		kraje_.clear();
		soorp_.clear();
		obce_.clear();

		auto zaciatokKraje = tabulkaKraje->begin();
		auto koniecKraje = tabulkaKraje->end();
		while (zaciatokKraje != koniecKraje)
		{
			delete zaciatokKraje.operator*().data_;
			zaciatokKraje.operator++();
		}
		
		auto zaciatokSoorp = tabulkaSoorp->begin();
		auto koniecSoorp = tabulkaSoorp->end();
		while (zaciatokSoorp != koniecSoorp)
		{
			delete zaciatokSoorp.operator*().data_;
			zaciatokSoorp.operator++();
		}

		auto zaciatokObce = tabulkaObce->begin();
		auto koniecObce = tabulkaObce->end();
		//int i = 0;
		while (zaciatokObce != koniecObce)
		{
			//std::cout << i << std::endl;
			//i++;
			delete zaciatokObce.operator*().data_;
			zaciatokObce.operator++();
		}
		

		tabulkaKraje->clear();
		delete tabulkaKraje;
		tabulkaSoorp->clear();
		delete tabulkaSoorp;
		tabulkaObce->clear();
		delete tabulkaObce;
		
		

		
		
	}
	
	void nacitaj(std::string nazovSuboru, ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>* hierarchy, std::vector<UzemnaJednotka*>& kraje, std::vector<UzemnaJednotka*>& soorp, std::vector<Obec*>& obce)
	{

		std::string nazovKraja;
		std::string kodKraja;
		bool najdiKodKraja = false;

		std::fstream subor;
		subor.open(nazovSuboru);
		std::string riadok;
		std::string udaj;

		std::string predoslyNazov;
		std::string nazovSOORP;
		std::string kodSOORP;

		//size_t krajeOrder = 0;
		//size_t soorpOrder = 0;
		//size_t obceOrder = 0;
		//auto root = *hierarchy.accessRoot();

		size_t iteraciaKraja = 0;
		size_t iteraciaSoorp = 0;
		size_t iteraciaObce = 0;

		
		ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>& root = hierarchy->emplaceRoot();
		hierarchy->accessRoot()->data_ = new UzemnaJednotka("CR", "0", TypUJ::republika);
		//std::cout << hierarchy->accessRoot()->data_->getNazovUzemnejJednotky();
		ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>* krajHierarchyBlock = hierarchy->accessRoot();
		ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>* soorpHierarchyBlock = hierarchy->accessRoot();
		while (std::getline(subor, riadok))
		{




			std::string nazovObce;
			std::string kodObce;
			std::string typObce;
			int katastralnaVymera = 0;
			int pocetObyv = 0;
			int pocetObyvDeti = 0;
			int pocetObyvSenior = 0;
			int kanalizacia = 0;
			int vodovod = 0;
			int plynovod = 0;
			int nuloveUdaje = 0;
			int cisloIteracie = 0;
			TypUJ _typ;
			std::stringstream stringStream(riadok);
			
			predoslyNazov = nazovSOORP;



			while (std::getline(stringStream, udaj, ';'))
			{
				if (najdiKodKraja && cisloIteracie == 0) {
					kodKraja = udaj;
					najdiKodKraja = false;
					_typ = TypUJ::kraj;
					UzemnaJednotka* poslednyKraj = new UzemnaJednotka(nazovKraja, kodKraja, _typ);
					krajHierarchyBlock = &hierarchy->emplaceSon(root, iteraciaKraja);
					//hierarchy->accessSon(root, iteraciaKraja)->data_ = poslednyKraj;
					krajHierarchyBlock->data_ = poslednyKraj;
					++iteraciaKraja;
					iteraciaSoorp = 0;
					kraje.push_back(poslednyKraj);
					//kraje_.push_back(poslednyKraj);
				}
				if (udaj.length() == 0)
				{
					++nuloveUdaje;
				}
				if (nuloveUdaje > 0 && cisloIteracie == 2)
				{
					nazovKraja = udaj;
					najdiKodKraja = true;
					break;
				}


				switch (cisloIteracie)
				{
				case 1:
					kodSOORP = udaj;
					break;
				case 2:
					nazovSOORP = udaj;
					break;
				case 4:
					nazovObce = udaj;
					break;
				case 5:
					kodObce = udaj;
					break;
				case 6:
					typObce = udaj;
					break;
				case 7:
					if (isdigit(udaj[0]))
					{
						katastralnaVymera = stoi(udaj);
					}
					else
					{
						katastralnaVymera = 0;
					}
					break;
				case 8:
					if (isdigit(udaj[0]))
					{
						pocetObyv = stoi(udaj);
					}
					else
					{
						pocetObyv = 0;
					}
					break;
				case 9:
					if (isdigit(udaj[0]))
					{
						pocetObyvDeti = stoi(udaj);
					}
					else
					{
						pocetObyvDeti = 0;
					}
					break;
				case 10:
					if (isdigit(udaj[0]))
					{
						pocetObyvSenior = stoi(udaj);
					}
					else
					{
						pocetObyvSenior = 0;
					}
					break;
				case 11:
					if (isdigit(udaj[0]))
					{
						kanalizacia = stoi(udaj);
					}
					else
					{
						kanalizacia = 0;
					}
					break;
				case 12:
					if (isdigit(udaj[0]))
					{
						vodovod = stoi(udaj);
					}
					else
					{
						vodovod = 0;
					}
					break;
				case 13:
					if (isdigit(udaj[0]))
					{
						plynovod = stoi(udaj);
					}
					else
					{
						plynovod = 0;
					}
					break;
				default:
					break;
				}
				++cisloIteracie;


			}
			if (predoslyNazov != nazovSOORP && kodSOORP != "")
			{
				_typ = TypUJ::soorp;
				UzemnaJednotka* poslednySoorp = new UzemnaJednotka(nazovSOORP, kodSOORP, _typ);
				soorp.push_back(poslednySoorp);
				//soorp_.push_back(poslednySoorp);
				soorpHierarchyBlock = &hierarchy->emplaceSon(*krajHierarchyBlock, iteraciaSoorp);
				//hierarchy->accessSon(*krajHierarchyBlock, iteraciaSoorp)->data_ = poslednySoorp;
				soorpHierarchyBlock->data_ = poslednySoorp;
				++iteraciaSoorp;
				iteraciaObce = 0;
				
			}

			if (nazovObce != "")
			{
				_typ = TypUJ::obec;
				Obec* poslednaObec = new Obec(nazovObce, kodObce, _typ, typObce, katastralnaVymera, pocetObyv, pocetObyvDeti, pocetObyvSenior, kanalizacia, vodovod, plynovod);
				obce.push_back(poslednaObec);
				//obce_.push_back(poslednaObec);
				auto* obceHierarchyBlock = &hierarchy->emplaceSon(*soorpHierarchyBlock, iteraciaObce);
				//hierarchy->accessSon(*soorpHierarchyBlock, iteraciaObce)->data_ = poslednaObec;
				obceHierarchyBlock->data_ = poslednaObec;
				++iteraciaObce;
				poslednaObec = nullptr;
				
			}


		}
		//delete[] hierarchy;
		//tempHiearrchy->clear();
		//delete hierarchy->accessRoot()->data_;
		subor.close();
	}
	void nacitajTabulky(std::vector<UzemnaJednotka*> kraje, std::vector<UzemnaJednotka*> soorpy, std::vector<Obec*> obce)
	{
		for (UzemnaJednotka* uzemnaJednotka : kraje)
		{
			if (!tabulkaKraje->contains(uzemnaJednotka->getNazovUzemnejJednotky()))
			{
				std::string kluc = uzemnaJednotka->getNazovUzemnejJednotky();
				auto list = new ds::adt::ImplicitList<UzemnaJednotka*>();
				list->insertLast(uzemnaJednotka);
				tabulkaKraje->insert(kluc, list);
			}
			else
			{
				auto list = tabulkaKraje->find(uzemnaJednotka->getNazovUzemnejJednotky());
				list->insertLast(uzemnaJednotka);
			}
		}
		std::cout << tabulkaKraje->size() << " " << kraje.size() << std::endl;


		for (UzemnaJednotka* uzemnaJednotka : soorpy)
		{
			if (!tabulkaSoorp->contains(uzemnaJednotka->getNazovUzemnejJednotky()))
			{
				std::string kluc = uzemnaJednotka->getNazovUzemnejJednotky();
				auto list = new ds::adt::ImplicitList<UzemnaJednotka*>();
				list->insertLast(uzemnaJednotka);
				tabulkaSoorp->insert(kluc, list);
			}
			else
			{
				auto list = tabulkaSoorp->find(uzemnaJednotka->getNazovUzemnejJednotky());
				list->insertLast(uzemnaJednotka);
			}
		}

		std::cout << tabulkaSoorp->size() << " " << soorpy.size() <<std::endl;

		for (Obec* uzemnaJednotka : obce)
		{
			if (!tabulkaObce->contains(uzemnaJednotka->getNazovUzemnejJednotky()))
			{
				std::string kluc = uzemnaJednotka->getNazovUzemnejJednotky();
				auto list = new ds::adt::ImplicitList<Obec*>();
				list->insertLast(uzemnaJednotka);
				tabulkaObce->insert(kluc, list);
			}
			else
			{
				auto list = tabulkaObce->find(uzemnaJednotka->getNazovUzemnejJednotky());
				list->insertLast(uzemnaJednotka);
			}
		}

		std::cout << tabulkaObce->size() << " " << obce.size() << std::endl;
		
	}

	ds::adt::GeneralBinarySearchTree<std::string, ds::adt::ImplicitList<UzemnaJednotka*>*, ds::adt::TableItem<std::string, ds::adt::ImplicitList<UzemnaJednotka*>*>>* getTabulkaKraje()
	{
		return tabulkaKraje;
	}

	ds::adt::GeneralBinarySearchTree<std::string, ds::adt::ImplicitList<UzemnaJednotka*>*, ds::adt::TableItem<std::string, ds::adt::ImplicitList<UzemnaJednotka*>*>>* getTabulkaSoorp()
	{
		return tabulkaSoorp;
	}

	ds::adt::GeneralBinarySearchTree<std::string, ds::adt::ImplicitList<Obec*>*, ds::adt::TableItem<std::string, ds::adt::ImplicitList<Obec*>*>>* getTabulkaObce()
	{
		return tabulkaObce;
	}

	template<typename T>
	void vypisTabulkuPodlaKluca(std::string key, ds::adt::GeneralBinarySearchTree<std::string, ds::adt::ImplicitList<T*>*, ds::adt::TableItem<std::string, ds::adt::ImplicitList<T*>*>>* tabulka)
	{
		if (tabulka->contains(key))
		{
			auto list = tabulka->find(key);
			auto zaciatok = list->begin();
			auto koniec = list->end();
			while (zaciatok != koniec)
			{
				std::cout << zaciatok.operator*()->getKodUzemnejJednotky() << " " << zaciatok.operator*()->getNazovUzemnejJednotky() << std::endl;
				zaciatok.operator++();
			}
		}
		else
		{
			std::cout << "Kluc sa nenachadza v tabulke" << std::endl;
		}
	}
	/*
	void nacitajHierarchiu(ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>* hierarchy, std::vector<UzemnaJednotka*> kraje, const std::vector<UzemnaJednotka*> soorp, const std::vector<UzemnaJednotka*> obce)
	{
		//auto hierarchy = new ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>();
		size_t poslednyKraj = 0;
		size_t poslednySoorp = 0;
		size_t poslednaObec = 0;
		auto root = hierarchy->emplaceRoot();

		root.data_ = new UzemnaJednotka("CR", "0", TypUJ::republika);
		std::cout << root.data_->getNazovUzemnejJednotky() << std::endl;
		for (size_t krajI = 0; krajI < kraje.size(); krajI++)
		{
			auto& krajH = hierarchy->emplaceSon(root, krajI);
			krajH.data_ = kraje.at(krajI);
			std::cout << krajH.data_->getNazovUzemnejJednotky();
			std::cout << hierarchy->accessSon(root, krajI)->data_->getNazovUzemnejJednotky() << std::endl;
			size_t soorpyKraja = 0;
			for (size_t soorpI = poslednySoorp; soorpI < soorp.size(); soorpI++)
			{
				auto soorpH = hierarchy->emplaceSon(krajH, soorpyKraja);
				soorpH.data_ = soorp.at(soorpI);
				poslednySoorp = soorpI;
				soorpyKraja++;
				size_t obceSoorpu = 0;
				for (size_t obceI = poslednaObec; obceI < obce.size(); obceI++)
				{
					auto obceH = hierarchy->emplaceSon(soorpH, obceSoorpu);
					obceH.data_ = obce.at(obceI);
					poslednaObec = obceI;
					obceSoorpu++;
				}
			}
		}
		
	}
	*/
};

