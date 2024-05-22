#pragma once

#include <libds/mm/memory_manager.h>
#include <libds/mm/memory_omanip.h>
#include <libds/constants.h>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ostream>
#include <utility>

namespace ds::mm {

    template<typename BlockType>
    class CompactMemoryManager : public MemoryManager<BlockType> {
    public:
        CompactMemoryManager();
        CompactMemoryManager(size_t size);
        CompactMemoryManager(const CompactMemoryManager<BlockType>& other);
        ~CompactMemoryManager() override;

        BlockType* allocateMemory() override;
        BlockType* allocateMemoryAt(size_t index);
        void releaseMemory(BlockType* pointer) override;
        void releaseMemoryAt(size_t index);
        void releaseMemory();

        size_t getCapacity() const;

        CompactMemoryManager<BlockType>& assign(const CompactMemoryManager<BlockType>& other);
        void changeCapacity(size_t newCapacity);
        void shrinkMemory();
        void clear();
        bool equals(const CompactMemoryManager<BlockType>& other) const;
        void* calculateAddress(const BlockType& data);
        size_t calculateIndex(const BlockType& data);
        BlockType& getBlockAt(size_t index);
        void swap(size_t index1, size_t index2);

        void print(std::ostream& os);

    private:
        size_t getAllocatedBlocksSize() const;
        size_t getAllocatedCapacitySize() const;

    private:
        BlockType* base_;
        BlockType* end_;
        BlockType* limit_;

        static const size_t INIT_SIZE = 4;
    };

    template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager() :
        CompactMemoryManager(INIT_SIZE)
    {
    }

    template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager(size_t size) :
        base_(static_cast<BlockType*>(std::calloc(size, sizeof(BlockType)))),
        end_(base_),
        limit_(base_ + size)
    {
    }

    template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager(const CompactMemoryManager<BlockType>& other) :
        CompactMemoryManager(other.getAllocatedBlockCount())
    {
        // TODO 02
        this->assign(other);
    }

    template<typename BlockType>
    CompactMemoryManager<BlockType>::~CompactMemoryManager()
    {
        // Ensures that destructor of each object is called.
        this->clear();
        std::free(base_);

        this->base_ = nullptr;
        this->end_ = nullptr;
        this->limit_ = nullptr;
        this->allocatedBlockCount_ = 0;
    }

    template<typename BlockType>
    BlockType* CompactMemoryManager<BlockType>::allocateMemory()
    {
        // TODO 02
        return this->allocateMemoryAt(static_cast<size_t>(this->end_ - this->base_));
    }

    template<typename BlockType>
    BlockType* CompactMemoryManager<BlockType>::allocateMemoryAt(size_t index)
    {
        // TODO 02
        if (this->end_ == this->limit_) {
            this->changeCapacity(this->getCapacity() > 0 ? (this->getCapacity() * 2) : this->INIT_SIZE);
        }
        BlockType* adr = &this->getBlockAt(index);
        if (index != this->allocatedBlockCount_) {
            std::memmove(adr + 1, adr, (this->end_ - adr) * sizeof(BlockType));
        }
        placement_new(adr);
        ++this->end_;
        ++this->allocatedBlockCount_;

        return adr;
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemory(BlockType* pointer)
    {
        // TODO 02
        BlockType* p = pointer;
        while (p != end_)
        {
            destroy(p);
            ++p;
        }

        end_ = pointer;
        MemoryManager<BlockType>::allocatedBlockCount_ = static_cast<size_t>(end_ - base_);
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemoryAt(size_t index)
    {
        // TODO 02
        BlockType* adr = this->base_ + index;
        destroy(adr);
        std::memmove(adr, adr + 1, (this->end_ - adr - 1) * sizeof(BlockType));
        --this->end_;
        --this->allocatedBlockCount_;
        

    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemory()
    {
        // TODO 02
        this->releaseMemoryAt(this->end_ - this->base_ - 1);
    }

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getCapacity() const
    {
        // TODO 02
        return limit_ - base_;
    }

    template<typename BlockType>
    CompactMemoryManager<BlockType>& CompactMemoryManager<BlockType>::assign
    (const CompactMemoryManager<BlockType>& other) //vraj DU
    {
        // TODO 02
        if (this != &other)
        {
            this->changeCapacity(other.getCapacity());
            this->clear();
            for (size_t i = 0; i < other.allocatedBlockCount_; ++i)
            {
                BlockType* adr = other.base_ + i;
                placement_copy(this->base_ + i, *adr);
            }
            this->allocatedBlockCount_ = other.allocatedBlockCount_;
            this->end_ = this->base_ + this->allocatedBlockCount_;
        }
        return *this;
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::shrinkMemory()
    {
        size_t newCapacity = static_cast<size_t>(end_ - base_);

        if (newCapacity < CompactMemoryManager<BlockType>::INIT_SIZE)
        {
            newCapacity = CompactMemoryManager<BlockType>::INIT_SIZE;
        }

        this->changeCapacity(newCapacity);
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::changeCapacity(size_t newCapacity)
    {
        // TODO 02
        if (newCapacity == this->getCapacity())
        {
            return;
        }

        if (newCapacity < this->getAllocatedBlockCount())
        {
            this->releaseMemory(base_ + newCapacity);
        }

        void* newBase = std::realloc(base_, newCapacity * sizeof(BlockType));

        if (newBase == nullptr)
        {
            throw std::bad_alloc();
        }

        base_ = static_cast<BlockType*>(newBase);
        end_ = base_ + MemoryManager<BlockType>::allocatedBlockCount_;
        limit_ = base_ + newCapacity;
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::clear()
    {
        // TODO 02
        this->releaseMemory(base_);
    }

    template<typename BlockType>
    bool CompactMemoryManager<BlockType>::equals(const CompactMemoryManager<BlockType>& other) const
    {
        return this == &other || 
            this->getAllocatedBlockCount() == other.getAllocatedBlockCount() && 
            std::memcmp(base_, other.base_, this->getAllocatedBlocksSize()) == 0;
    }

    template<typename BlockType>
    void* CompactMemoryManager<BlockType>::calculateAddress(const BlockType& data)
    {
        
        BlockType* cur = base_;

        while (cur != end_ && cur != &data)
        {
            ++cur;
        }

        return cur == end_ ? nullptr : cur;
    }

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::calculateIndex(const BlockType& data)
    {
       
        if (&data >= this->base_ && &data < this->end_) {
            return &data - this->base_;
        }
        else {
            return INVALID_INDEX;
        }
    }

    template<typename BlockType>
    BlockType& CompactMemoryManager<BlockType>::getBlockAt(size_t index)
    {
        return *(this->base_ + index); //BlockType&
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::swap(size_t index1, size_t index2)
    {
        std::swap(this->getBlockAt(index1), this->getBlockAt(index2));
    }

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getAllocatedBlocksSize() const
    {
        return (this->end_ - this->base_) * sizeof(BlockType);
    }

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getAllocatedCapacitySize() const
    {
        return this->limit_ - this->base_;
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::print(std::ostream& os)
    {
        os << "first = " << base_ << std::endl;
        os << "last = " << end_ << std::endl;
        os << "limit = " << limit_ << std::endl;
        os << "block size = " << sizeof(BlockType) << "B" << std::endl;

        BlockType* ptr = base_;
        while (ptr != limit_)
        {
            std::cout << ptr;
            os << PtrPrintBin<BlockType>(ptr);

            if (ptr == base_) {
                os << "<- first";
            }
            else if (ptr == end_) {
                os << "<- last";
            }
            os << std::endl;
            ++ptr;
        }

        os << limit_ << "|<- limit" << std::endl;
    }

}