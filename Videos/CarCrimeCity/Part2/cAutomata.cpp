#include "cAutomata.h"


cAuto_Node::cAuto_Node()
{
	pos = { 0,0 };
}

cAuto_Node::cAuto_Node(const olc::vf2d &worldpos)
{
	pos = worldpos;
}

olc::vf2d cAuto_Track::GetPostion(float t, cAuto_Node *pStart)
{
	// pStart indicates the node the automata first encounted this track
	if (node[0] == pStart)
	{
		return node[0]->pos + (node[1]->pos - node[0]->pos) *  (t / fTrackLength);
	}
	else
	{
		return node[1]->pos + (node[0]->pos - node[1]->pos) *  (t / fTrackLength);
	}
}



cAuto_Body::cAuto_Body()
{
}


cAuto_Body::~cAuto_Body()
{
}


void cAuto_Body::UpdateAuto(float fElapsedTime)
{
	// Work out which node is the target destination
	cAuto_Node *pExitNode = pCurrentTrack->node[0];
	if (pExitNode == pTrackOriginNode)
		pExitNode = pCurrentTrack->node[1];

	bool bAutomataCanMove = true;

	float fDistanceToAutoInFront = 1.0f;

	// First check if the vehicle overlaps with the one in front of it

	// Get an iterator for this automata
	auto itThisAutomata = std::find(pCurrentTrack->listAutos.begin(), pCurrentTrack->listAutos.end(), this);

	// If this automata is at the front of this track segment
	if (*itThisAutomata == pCurrentTrack->listAutos.front())
	{
		// Then check all the following track segments. Take the position of
		// each vehicle at the back of the track segments auto list
		for (auto &track : pExitNode->listTracks)
		{
			if (track != pCurrentTrack && !track->listAutos.empty())
			{
				// Get Auto at back
				float fDistanceFromTrackStartToAutoRear = track->listAutos.back()->fAutoPos - track->listAutos.back()->fAutoLength;

				if ((*itThisAutomata)->fAutoPos < (pCurrentTrack->fTrackLength + fDistanceFromTrackStartToAutoRear - fAutoLength))
				{
					// Move Automata along track, as there is space
					//bAutomataCanMove = true;
					fDistanceToAutoInFront = (pCurrentTrack->fTrackLength + fDistanceFromTrackStartToAutoRear - 0.1f) - (*itThisAutomata)->fAutoPos;
				}
				else
				{
					// No space, so do not move automata
					bAutomataCanMove = false;
				}
			}
			else
			{
				// Track in front was empty, node is clear to pass through so
				//bAutomataCanMove = true;
			}
		}



	}
	else
	{
		// Get the automata in front
		auto itAutomataInFront = itThisAutomata;
		itAutomataInFront--;

		// If the distance between the front of the automata in front and the fornt of this automata
		// is greater than the length of the automata in front, then there is space for this automata
		// to enter
		if (fabs((*itAutomataInFront)->fAutoPos - (*itThisAutomata)->fAutoPos) > ((*itAutomataInFront)->fAutoLength + 0.1f))
		{
			// Move Automata along track
			//bAutomataCanMove = true;
			fDistanceToAutoInFront = ((*itAutomataInFront)->fAutoPos - (*itAutomataInFront)->fAutoLength - 0.1f) - (*itThisAutomata)->fAutoPos;
		}
		else
		{
			// No space, so do not move automata
			bAutomataCanMove = false;
		}
	}

	if (bAutomataCanMove)
	{
		if (fDistanceToAutoInFront > pCurrentTrack->fTrackLength) fDistanceToAutoInFront = pCurrentTrack->fTrackLength;
		fAutoPos += fElapsedTime * std::max(fDistanceToAutoInFront, 1.0f) * (fAutoLength < 0.1f ? 0.3f : 0.5f);
	}


	if (fAutoPos >= pCurrentTrack->fTrackLength)
	{
		// Automata has reached end of current track

		// Check if it can transition beyond node
		if (!pExitNode->bBlock)
		{
			// It can, so reset position along track back to start
			fAutoPos -= pCurrentTrack->fTrackLength;

			// Choose a track from the node not equal to this one, that has an unblocked exit node

			// For now choose at random
			cAuto_Track *pNewTrack = nullptr;

			if (pExitNode->listTracks.size() == 2)
			{
				// Automata is travelling along straight joined sections, one of the 
				// tracks is the track its just come in on, the other is the exit, so
				// choose the exit.
				auto it = pExitNode->listTracks.begin();
				pNewTrack = (*it);
				if (pCurrentTrack == pNewTrack)
				{
					++it;
					pNewTrack = (*it);
				}
			}
			else
			{
				// Automata has reached a junction with several exits
				while (pNewTrack == nullptr)
				{
					int i = rand() % pExitNode->listTracks.size();
					int j = 0;
					for (auto it = pExitNode->listTracks.begin(); it != pExitNode->listTracks.end(); ++it)
					{
						cAuto_Track* track = (*it);

						// Work out which node is the target destination
						cAuto_Node *pNewExitNode = track->node[0];
						if (pNewExitNode == pExitNode)
							pNewExitNode = track->node[1];

						if (j == i && track != pCurrentTrack && !pNewExitNode->bBlock /*((*it)->cell != pCurrentTrack->cell)*/)
						{
							pNewTrack = track;
							break;
						}

						j++;
					}
				}
			}


			// Change to new track, the origin node of the next
			// track is the same as the exit node to the current track
			pTrackOriginNode = pExitNode;

			// Remove the automata from the front of the queue
			// on the current track
			pCurrentTrack->listAutos.pop_front();

			// Switch the automatas track link to the new track
			pCurrentTrack = pNewTrack;

			// Push the automata onto the back of the new track queue
			pCurrentTrack->listAutos.push_back(this);

		}
		else
		{
			// It cant pass the node, so clamp automata at this location
			fAutoPos = pCurrentTrack->fTrackLength;
		}

	}
	else
	{
		// Automata is travelling
		vAutoPos = pCurrentTrack->GetPostion(fAutoPos, pTrackOriginNode);
	}
}





