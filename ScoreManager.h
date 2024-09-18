#pragma once
class ScoreManager{
public:

	ScoreManager();
	void IncreaseScore(int points);
	int ShowScore();

private:
	int score;
};

