#include "GP3CameraShake.h"

UGP3CameraShake::UGP3CameraShake()
{
    OscillationDuration = -1.0f;
    OscillationBlendInTime = 0.075f;
    OscillationBlendOutTime = 1.0f;
    bSingleInstance = true;

    RotOscillation.Pitch.Amplitude = 0.7f;
    RotOscillation.Pitch.Frequency = 4.0f;
    RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
    RotOscillation.Pitch.Waveform = EOscillatorWaveform::PerlinNoise;

    RotOscillation.Yaw.Amplitude = 0.6f;
    RotOscillation.Yaw.Frequency = 1.8f;
    RotOscillation.Yaw.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
    RotOscillation.Yaw.Waveform = EOscillatorWaveform::PerlinNoise;

    RotOscillation.Roll.Amplitude = 0.6f;
    RotOscillation.Roll.Frequency = 1.8f;
    RotOscillation.Roll.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
    RotOscillation.Roll.Waveform = EOscillatorWaveform::PerlinNoise;

    LocOscillation.X.Amplitude = 0.7f;
    LocOscillation.X.Frequency = 0.7f;
    LocOscillation.X.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
    LocOscillation.X.Waveform = EOscillatorWaveform::PerlinNoise;
    
    LocOscillation.Y.Amplitude = 0.7f;
    LocOscillation.Y.Frequency = 0.7f;
    LocOscillation.Y.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
    LocOscillation.Y.Waveform = EOscillatorWaveform::PerlinNoise;

    LocOscillation.Z.Amplitude = 0.7f;
    LocOscillation.Z.Frequency = 0.7f;
    LocOscillation.Z.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
    LocOscillation.Z.Waveform = EOscillatorWaveform::PerlinNoise;
}
    
