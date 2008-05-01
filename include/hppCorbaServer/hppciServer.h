/*
  Research carried out within the scope of the Associated International Laboratory: Joint Japanese-French Robotics Laboratory (JRL)

  Developed by Florent Lamiraux (LAAS-CNRS)

*/

#ifndef HPPCI_SERVER_H
#define HPPCI_SERVER_H

#include "hppCore/hppPlanner.h"
#include "kwsPlusSteeringMethodFactory.h"
#include "kwsPlusDistanceFactory.h"

class ChppciServerPrivate;

/** 

The Corba server of Library Hpp is mainly implemented by class ChppciServer. This class basically:
\li initializes a Corba server that implements several interfaces defined by the following modules,
\li instantiate an object ChppPlanner a pointer to which is stored in ChppciServer::hppPlanner.
The Corba interfaces trigger actions processed by ChppPlanner object. More information is provided in each interface documentation.
 */

/** \brief Implementation of Hpp module Corba server.
 *
This class initializes the Corba server and starts the following Corba interface implementations.
\li ChppciServer::robotServant,
\li ChppciServer::obstacleServant,
\li ChppciServer::problemServant.

To use this object, call the constructor
\code
int argc=1;
char *argv[1] = {"program"};
ChppPlanner* hppPlanner = new ChppPlanner;
ChppciServer server(ChppPlanner* hppPlanner, argc, argv);
\endcode
After starting a name server and configuring your Corba implementation, start the servers.
\code
server.startCorbaServer();
\endcode
Then, enter in the loop that handle the Corba requests
\code
server.processRequest(true);
\endcode
You can then send request to the servers.
*/

class ChppciServer {
public:
  /** 
      \brief Constructor 
      \param inHppPlanner the object that will handle Corba requests.
      \param argc, argv parameter to feed ORB initialization. 
      
      \note It is recommended to configure your Corba implementation through environment variables and to set argc to 1 and argv to any string.
  */
  ChppciServer(ChppPlanner* inHppPlanner, int argc, char *argv[]);
  /// \brief Shutdown CORBA server
  ~ChppciServer();
  /// \brief Initialize CORBA server to process requests from clients to hpp module
  /// \return 0 if success, -1 if failure.
  int startCorbaServer();
  /// \brief If ORB work is pending, process it
  /// \param loop if true, the function never returns; if false, the function processes pending requests and returns.
  int processRequest(bool loop);
  /// \brief return a pointer to object ChppciServer::hppPlanner.
  ChppPlanner *getHppPlanner();

  // 
  // Static public
  // 
  static ChppciServer* getInstance();

  /**
     \name Steering method management
     @{
  */
  
  /**
     \brief Indicates whether a name already corresponds to a steering method factory.
     \param inName Name of the steering method factory.
     \return true if name already corresponds to a steering method factory.
  */
  bool steeringMethodFactoryAlreadySet(std::string inName);

  /**
     \brief Add a steering method factory
     \param inName Name of the steering method built by factory.
     \param inSteeringMethodFactory Steering method factory
     \return true if success, false if name already used
  */
  bool addSteeringMethodFactory(std::string inName, 
				CkwsPlusSteeringMethodFactory* inSteeringMethodFactory);

  /**
     \brief Get steering method from name.
  */
  CkwsSteeringMethodShPtr createSteeringMethod(std::string inName, bool inOriented);

  /**
     @}
  */

  /**
     \name Distance function management
  */
  /**
     \brief Indicates whether a name already corresponds to a distance factory.
     \param inName Name of the distance function factory.
     \return true if name already corresponds to a distance function factory.
  */
  bool distanceFactoryAlreadySet(std::string inName);

  /**
     \brief Add a distance function factory
     \param inName Name of the distance function built by factory.
     \param inDistanceFactory distance function factory
     \return true if success, false if name already used
  */
  bool addDistanceFactory(std::string inName, 
			  CkwsPlusDistanceFactory* inDistanceFactory);

  /**
     \brief Get distance function from name.
  */
  CkwsDistanceShPtr createDistanceFunction(std::string inName, bool inOriented);

  /**
     @}
  */

private:

  /**
     \name CORBA server initialization
  */

  /**
     \brief Initialize ORB and CORBA servers.
  */
  ktStatus initORBandServers(int argc, char *argv[]);

  /**
     @}
  */

  /**
     \name Steering method factories
     @{
  */
  /**
     \brief Initialize map of steering method factories.

     Insert default factories: 
     \li "linear" factory building linear steering methods,
     \li "rs" factory building Reeds and Shepp steering methods (radius = 1),
     \li "flic" factory building flat interpolation based steering methods.
  */
  void initMapSteeringMethodFactory();
  
  /**
     \brief Destroy each steering method factory stored in the map.
  */
  void destroySteeringMethodFactories();

  /**
     \brief Associative array of Steering method factories.
  */
  std::map<std::string, CkwsPlusSteeringMethodFactory*> attMapSteeringMethodFactory;

  /**
     @}
  */
  /**
     \name Distance function factories
     @{
  */
  /**
     \brief Initialize map of distance function factories.

     Insert default factories: 
     \li "linear" factory building linear distance functions,
     \li "rs" factory building Reeds and Shepp distance functions (radius = 1),
     \li "flic" factory building flat interpolation based distance functions.
  */
  void initMapDistanceFunctionFactory();
  
  /**
     \brief Destroy each distance function factory stored in the map.
  */
  void destroyDistanceFunctionFactories();

  /**
     \brief Associative array of distance function factories.
  */
  std::map<std::string, CkwsPlusDistanceFactory*> attMapDistanceFunctionFactory;

  /**
     @}
  */

  ChppciServerPrivate* attPrivate;
  /// \brief static pointer to the only object of this class.
  static ChppciServer* s_hppciServer;

  /// \brief pointer to ChppPlanner Object.
  /// At initialization, the constructor creates a ChppPlanner object and keeps a pointer to it. All Corba requests are processed by this object. Notice that this pointer is passed to each constructor of implementation classes of the server Corba interface.
  ChppPlanner *hppPlanner;


};

#endif