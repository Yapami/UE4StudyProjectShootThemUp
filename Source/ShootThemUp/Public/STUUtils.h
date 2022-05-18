#pragma once

class STUUtils
{
public:
	template <typename T>
    static T* GetSTUPlayerComponent(APawn* PlayerPawn)
    {
        if (!PlayerPawn)
        {
            return false;
        }
        return Cast<T>(PlayerPawn->GetComponentByClass(T::StaticClass()));
    }
};
