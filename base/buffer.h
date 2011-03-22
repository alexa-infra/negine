#pragma once

class BufferDescription;

class Buffer 
{
    BufferDescription desc;
public:
    explicit Buffer(const BufferDesription& desc);
    ~Buffer();

    void Write(u32 elements);

private:
    DISALLOW_COPY_AND_ASSIGN(Buffer);
};
