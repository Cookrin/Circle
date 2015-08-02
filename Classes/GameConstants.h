//
// GameConstants.h
// DotWar
//
// Created by ryutamaki on 2015/07/17.
//
//

#ifndef DotWar_GameConstants_h
#define DotWar_GameConstants_h

static const float SCENE_TRANSITION_DURATION = 0.5f;
static const Color3B SCENE_TRANSITION_COLOR = Color3B(CIRCLE_GRAY);

static const float BATTLE_FIELD_FRAME_THICKNESS = 5.0f;

static const float SENSITIVITY_TO_CONTROL_PLAYER = 10.0f;

enum class GameState
{
    PREPARE,
    PLAYING,
    RESULT,
};

#endif
