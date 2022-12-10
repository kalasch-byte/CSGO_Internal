#pragma once

#include <cstdint>
#include <winnt.h>

class EasyVMT
{
private:
    uintptr_t* pVmt;
public:
    uintptr_t** ppVTableAddress = nullptr;
    uintptr_t* pOriginalAddress = nullptr;
    size_t      uFunctionCount = 0;

    // ----------------------------------------------------------------------------------------------------------------------

    inline unsigned int GetVirtualFunction(void* pClassBase, uint16_t uIndex)
    {
        return static_cast<unsigned int>((*static_cast<int**>(pClassBase))[uIndex]);
    }

    // ----------------------------------------------------------------------------------------------------------------------

    inline EasyVMT(void* pClassBase)
    {
        ppVTableAddress = reinterpret_cast<uintptr_t**>(pClassBase);

        // Acquire the amount of functions inside the virtual class.
        while (reinterpret_cast<uintptr_t*>(*ppVTableAddress)[uFunctionCount])
        {
            uFunctionCount++;
        }

        // Set the original.
        pOriginalAddress = *ppVTableAddress;

        pVmt = new uintptr_t[uFunctionCount + 1];

        // Make a copy of the entire vTable.
        memcpy(pVmt, &pOriginalAddress[-1], (sizeof(uintptr_t) * uFunctionCount) + sizeof(uintptr_t));
    }

    // ----------------------------------------------------------------------------------------------------------------------

    inline void Hook(void* detourFunction, size_t functionIndex)
    {
        // A simple sanity check to ensure that are hook is valid and in valid boundaries.
        bool bValidHook = detourFunction != nullptr && functionIndex >= 0 && functionIndex <= uFunctionCount;

        if (bValidHook)
        {
            pVmt[functionIndex + 1] = reinterpret_cast<uintptr_t>(detourFunction);
            *ppVTableAddress = &pVmt[1];
        }
    }

    // ----------------------------------------------------------------------------------------------------------------------

    inline void Unhook()
    {
        *ppVTableAddress = pOriginalAddress;
        delete pVmt;
    }

    // ----------------------------------------------------------------------------------------------------------------------

    inline uintptr_t* GetOriginalAddress()
    {
        if (pOriginalAddress != nullptr)
            return pOriginalAddress;
    }

    // ----------------------------------------------------------------------------------------------------------------------

    template<typename Fn>
    inline Fn GetOriginalMethod(size_t uMethodIndex)
    {
        return reinterpret_cast<Fn>(pOriginalAddress[uMethodIndex]);
    }

    // ----------------------------------------------------------------------------------------------------------------------
};