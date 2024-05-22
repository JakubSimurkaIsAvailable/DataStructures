#pragma once
#include <string>
#include <libds/heap_monitor.h>

enum TypUJ { obec, soorp, kraj, republika };

class UzemnaJednotka
{
private:
    std::string nazovUzemnejJednotky;
    std::string kodUzemnejJednotky;
    TypUJ typ;
public:
    
 
    UzemnaJednotka(std::string _nazovUzemnejJednotky, std::string _kodUzemnejJednotky, TypUJ _typ)
    {
        nazovUzemnejJednotky = _nazovUzemnejJednotky;
        kodUzemnejJednotky = _kodUzemnejJednotky;
        typ = _typ;
    }

    ~UzemnaJednotka()
    {
    	nazovUzemnejJednotky.clear();
		kodUzemnejJednotky.clear();
    }

    std::string getNazovUzemnejJednotky() const
    {
        return nazovUzemnejJednotky;
    }

    std::string getKodUzemnejJednotky() const
    {
        return kodUzemnejJednotky;
    }

    TypUJ getTyp() const
    {
        return typ;
    }

    std::string getTypString() const
    {
    	switch (typ)
    	{
    	case obec:
			return "obec";
		case soorp:
			return "soorp";
		case kraj:
			return "kraj";
		case republika:
			return "republika";
		default:
			return "neplatny typ";
			
		}
	}
};

class Obec : public UzemnaJednotka
{
private:
	std::string typObce;
	int katastralnaVymera;
	int pocObyv;
	int pocObyvDeti;
	int pocObyvSenior;
	int kanalizacia;
	int vodovod;
	int plynovod;
public:
	Obec(std::string _nazovUzemnejJednotky, std::string _kodUzemnejJednotky, TypUJ _typ,std::string _typObce, int _katastralnaVymera, int _pocObyv, int _pocObyvDeti, int _pocObyvSenior, int _kanalizacia, int _vodovod, int _plynovod)
		: UzemnaJednotka(_nazovUzemnejJednotky, _kodUzemnejJednotky, _typ)
	{
		typObce = _typObce;
		katastralnaVymera = _katastralnaVymera;
		pocObyv = _pocObyv;
		pocObyvDeti = _pocObyvDeti;
		pocObyvSenior = _pocObyvSenior;
		kanalizacia = _kanalizacia;
		vodovod = _vodovod;
		plynovod = _plynovod;
	}

    


};

