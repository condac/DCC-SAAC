// Define optional parts and configurations here


//===========================================================================
//================================= Trains  =================================
//===========================================================================



#define MAX_TRAINS 10 // with 10 trains the performance is about 2-3 updates/s
Train trains[MAX_TRAINS] {Train( 1, "RC1    ", DCC), 
                  Train( 3, "RC2    ", DCC), 
                  Train( 72, "ST72    ", DCC), 
                  Train( 2, "nr2", DCC), 
                  Train( 4, "nr4", DCC), 
                  Train( 24, "Electric", UNCONF), //0220
                  Train( 60, "Railcar", UNCONF),  //0202
                  Train( 72, "Diesel", UNCONF),   //0022
                  Train( 78, "Steam", UNCONF),    //0222
                  Train( 80, "Delta Pilot", UNCONF), 
                  
                  };


//===========================================================================
//================================= IR Remote  ==============================
//===========================================================================

// Comment / Uncomment #define IRSUPPORT to enable or disable support for IR remote
#define IRSUPPORT

// Look in remotes.h for remote options, or create and map your own remote like in remote_basic1.h
#define IR_REMOTE BASIC1






