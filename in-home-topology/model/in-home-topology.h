/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __INHOMETOPOLOGY_H__
#define __INHOMETOPOLOGY_H__

#include <vector>
#include <deque>
#include <stdint.h>
#include <math.h>
#include "ns3/plc-channel.h"

#define DERIVATION_BOX_ID       0
#define PLC_MODEM_ENERGY        1

namespace ns3
{

/*
 * These classes are supported with data and ideas from following publications:
 *
 * 1. Bottom-Up Statistical PLC Channel Modeling—Part I: Random Topology Model and Efficient Transfer Function Computation
 *    Andrea M. Tonello, Member, IEEE, and Fabio Versolatto, Student Member, IEEE
 *
 * 2. Energie-Info BDEW Bundesverband der Energie- und Wasserwirtschaft e.V. Stromverbrauch im Haushalt, Berlin, Oktober 2013
 *
 * 3. Ausstattung mit Gebrauchsgütern. Ausstattung privater Haushalte mit Haushaltsß und sonstigen Geräten Deutschland. Statistisches Bundesamt.
 */

struct Coordinate
{
  double x;
  double y;
};

struct Dev
{
  /*
   * dev id
   */
  uint16_t id;
  /*
   * id of neighbour dev
   */
  uint16_t neighbour;
  /*
   * index of node that associates with this dev
   */
  int16_t nodeIndex;
  Coordinate c;
  double energy;
};

enum InhomeTopologyType
{
  START_TOPOLOGY_TYPE, START_PERIMETER_TOPOLOGY_TYPE, BUS_TOPOLOGY_TYPE
};

class Room
{
public:
  Room (double L);
  virtual
  ~Room ();

  void
  AddDevice (uint16_t devId)
  {
    Dev dev;
    dev.id = devId;
    m_dev.push_back (dev);
  }

  void
  Create ();

  std::deque<Dev>
  GetDevs ()
  {
    return m_dev;
  }

private:

  double
  GetLength (double maxLength);
  /*
   * Verteilungsbox / Anschlusskasten
   */
  Dev m_vb;
  /*
   * indexes of electrical devices
   */
  std::deque<Dev> m_dev;

  /*
   * room width/length
   * unit [m]
   */
  double m_L;

  InhomeTopologyType m_topologyType;
};

class InhomeTopology
{
public:

  InhomeTopology (double totalSquare, uint16_t numModems, uint16_t numDevs);
  virtual
  ~InhomeTopology ();

  std::vector<Dev>
  GetPositions ()
  {
    return m_devs;
  }

  void
  Create (PLC_NodeList &nodes, Ptr<PLC_Cable> cable);

private:

  void
  CreateRooms (uint16_t roomNum);

  uint16_t
  GetNumPersons ();
  /*
   * calculate total consumption of a flat/house in kWh per year
   */
  double
  GetTotalEnery (uint16_t numPerson);
//  void
//  InitDevs (double totalkWh);
  void
  InitDevs ();
  void
  InitModems();
  /*
   * all rooms of the same size and have a form of a square
   * unit [m2]
   */
  double m_roomSqaure;
  double m_L;
  /*
   * i - room number
   * j - device index (referred to m_devEnergy);
   */
  std::vector<Room*> m_rooms;

  std::vector<double> m_devEnergy;

  std::vector<Dev> m_devs;
  std::vector<Coordinate> m_roomOrigins;

  uint16_t m_numModems;
  uint16_t m_numDevs;
};

}

#endif /* __IN_HOME_TOPOLOGY_H__ */

