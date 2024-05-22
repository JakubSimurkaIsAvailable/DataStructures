#pragma once
#pragma once

#include <complexities/complexity_analyzer.h>
#include <list>
#include <random>
#include <vector>

namespace ds::utils
{
    /**
     * @brief Common base for list analyzers.
     */
    template<class Table>
    class TableAnalyzer : public ComplexityAnalyzer<List>
    {
    protected:
        explicit TableAnalyzer(const std::string& name);

    protected:
        void growToSize(Table& structure, size_t size) override;
        int key_;
        int data_;
       

    private:
        std::default_random_engine rngData_;
        std::default_random_engine rngKey_;
    };

    /**
     * @brief Analyzes complexity of an insertion at the beginning.
     */
    template<class Table>
    class TableInsertAnalyzer : public TableAnalyzer<Table>
    {
    public:
        explicit TableInsertAnalyzer(const std::string& name);

    protected:
        void executeOperation(Table& structure) override;
    };

    /**
     * @brief Analyzes complexity of an erasure at the beginning.
     */
    template<class Table>
    class TableRemoveAnalyzer : public TableAnalyzer<Table>
    {
    public:
        explicit TableRemoveAnalyzer(const std::string& name);

    protected:
        void executeOperation(Table& structure) override;
    };

    /**
     * @brief Container for all list analyzers.
     */
    class TablesAnalyzer : public CompositeAnalyzer
    {
    public:
        TablesAnalyzer();
    };

    //----------

    template<class Table>
    TableAnalyzer<Table>::TableAnalyzer(const std::string& name) :
        ComplexityAnalyzer<Table>(name),
        rngData_(144),
        rngKey_(144),
		key_(0),
		data_(0)
    {
        this->registerBeforeOperation(
            [this](Table& table)
            {
                std::uniform_int_distribution dist(1, 1000000);
                this->key_ = dist(this->rngKey_);
                while (table.contains(this->key_))
				{
					this->key_ = dist(this->rngKey_);
				}
                this->data_ = dist(this->rngData_);
            }
        );
    }

    template <class Table>
    void TableAnalyzer<Table>::growToSize(Table& structure, size_t size)
    {
        size_t count = size - structure.size();
        for (size_t i = 0; i < count; ++i)
		{
            std::uniform_int_distribution dist(1, 1000000);
			int key = dist(this->rngKey_);
            int data = dist(this->rngData_);
            while(structure.contains(key))
			{
				key = dist(this->rngKey_);
			}
            structure.insert(key, data);

		}
    }


    //----------

    template <class Table>
    TableInsertAnalyzer<Table>::TableInsertAnalyzer(const std::string& name) :
        TableAnalyzer<Table>(name)
    {

    }

    template <class Table>
    void TableInsertAnalyzer<Table>::executeOperation(Table& structure)
    {
        structure.insert(this->key_, this->data_);
    }

    //----------

    template <class Table>
    TableRemoveAnalyzer<Table>::TableRemoveAnalyzer(const std::string& name) :
        TableAnalyzer<Table>(name)
    {

    }

    template <class Table>
    void TableRemoveAnalyzer<Table>::executeOperation(Table& structure)
    {

        // po implementacii vymazte vyhodenie vynimky!
        structure.erase(structure.begin());
    }

    //----------

    inline TablesAnalyzer::TablesAnalyzer() :
        CompositeAnalyzer("Lists")
    {
        this->addAnalyzer(std::make_unique<ListInsertAnalyzer<std::vector<int>>>("vector-insert"));
        this->addAnalyzer(std::make_unique<ListInsertAnalyzer<std::list<int>>>("list-insert"));
        this->addAnalyzer(std::make_unique<ListRemoveAnalyzer<std::vector<int>>>("vector-remove"));
        this->addAnalyzer(std::make_unique<ListRemoveAnalyzer<std::list<int>>>("list-remove"));
    }
}
