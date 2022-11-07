#pragma once

class STUUtils
{
public:
	template <typename T>
    static T* GetSTUPlayerComponent(AActor* PlayerPawn)
    {
        if (!PlayerPawn)
        {
            return false;
        }
        return Cast<T>(PlayerPawn->GetComponentByClass(T::StaticClass()));
    }
};
