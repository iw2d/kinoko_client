#pragma once
#pragma pack(push, 8)

#include "IWzShape2D.h"
#include "ztl/zcom.h"

struct __declspec(uuid("864e03be-8312-4391-b9e8-ee20ee32fb4e"))
/* interface */ IWzConvex2D;

_COM_SMARTPTR_TYPEDEF(IWzConvex2D, __uuidof(IWzConvex2D));

struct __declspec(uuid("864e03be-8312-4391-b9e8-ee20ee32fb4e"))
IWzConvex2D : IWzShape2D
{};

#pragma pack(pop)
