
class Mover
{

  public:
	Mover(int pixelsAmount);
  
	int GetCurPos();
	int GetOldPos();
	void moveForward();
	void moveBackwards();
	void moveDistance(int distance);
	
	void performMove();
  
  private:
	int m_pixels;
  
	int curDistance;
  
    int oldPos;
    int curPos;
	void moveAmount(int amount);

};
