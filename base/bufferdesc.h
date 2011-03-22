#pragma once

struct BufferElement
{
    u32 Type;
    u32 Size;
    u32 Count;
    u32 Offset;
};
typedef std::vector<BufferElement> BufferElementList;

class BufferDescription
{
private:
    u32 stride_;
    BufferElementList elements_;
    u32 count_;

public:
    BufferDescription() : stride_(0), count_(0) {}

    explicit BufferDescription(const BufferElementList& elements)
        : elements_(elements), stride_(0), count_(0)
    {
        Update();
    }

    void AddElement(u32 type, u32 size, u32 count = 1)
    {
        BufferElement element;
        element.Type = type;
        element.Size = size;
        element.Count = count;
        element.Offset = 0;
        elements_.push_back(element);
        Update();
    }

    template<typename TT>
    void AddElement(u32 type, u32 count = 1)
    {
        AddElement(type, sizeof(TT), count);
    }

    const BufferElementList& getElements() const { return elements_; }
    void setElements(const BufferElementList& list) { elements_ = list; Update(); }

    u32 getStride() const { return stride_; }

    void setCount(u32 count) { count_ = count; }
    u32 getCount() const { return count_; }

    u32 getFullSize() const { return count_ * stride; }

private:
    void Update()
    {
        u32 offset = 0;
        for (BufferElementList::iterator it = elements_.begin();
            it != elements_.end();
            ++it)
        {
            (*it).Offset = offset;
            offset += (*it).Size * (*it).Count;
        }
        stride_ = offset;
    }

private:
    DISALLOW_COPY_AND_ASSIGN(BufferDescription);
};
