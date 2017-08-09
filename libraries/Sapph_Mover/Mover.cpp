#include "Mover.h"

Mover::Mover(int pixelsAmount)
{
	m_pixels = pixelsAmount;
	curPos = 0;
	oldPos = 0;
	curDistance = 0;
}


void Mover::performMove()
{
	if (curDistance > 0)
	{
		moveForward();
		curDistance--;
	}
	else if (curDistance < 0)
	{
		moveBackwards();
		curDistance++;
	}
}


int Mover::GetCurPos()
  {
    return curPos;
  }

int Mover::GetOldPos()
  {
    return oldPos;
  }

void Mover::moveForward()
  {
    moveAmount(1);
  }

  void Mover::moveBackwards()
  {
    moveAmount(-1);
  }
  
  void Mover::moveDistance(int distance)
  {
	  if (curDistance == 0)
	  {
		  curDistance = distance;
	  }
  }
  
  void Mover::moveAmount(int amount)
  {
    oldPos = curPos;
    curPos += amount;

    if (curPos >= m_pixels)
    {
      curPos = curPos % m_pixels;
    }
    else if (curPos < 0)
    {
      curPos = -1 * curPos;
      curPos = m_pixels - curPos;
      curPos = curPos % m_pixels;
    }
  }