#pragma once

class AnimUtils
{
public:
    template<typename T>
    static T* FindFirsNotifyByClass(UAnimSequenceBase* Anim)
    {
        if (!Anim)
        {
            return nullptr;
        }
        const auto NotifyEvents = Anim->Notifies;
        for (auto NotifyEvent: NotifyEvents)
        {
            auto AnimNotify = Cast<T>(NotifyEvent.Notify);
            if (AnimNotify)
            {
                return AnimNotify;
            }
        }
        return nullptr;
    }
};