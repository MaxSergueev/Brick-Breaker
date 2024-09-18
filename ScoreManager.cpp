#include "ScoreManager.h"

ScoreManager::ScoreManager() {
	score = 0;
}

void ScoreManager::IncreaseScore(int points) {
	score += points;
}

int ScoreManager::ShowScore() {
	return score;
}