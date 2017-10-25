// Define optional parts and configurations here


//===========================================================================
//================================= Trains  =================================
//===========================================================================



#define MAX_TRAINS 10 // with 10 trains the performance is about 2-3 updates/s
Train trains[MAX_TRAINS] {
                  Train( 1  , "RC1       ", DCC), 
                  Train( 3  , "RC2       ", DCC), 
                  Train( 72 , "ST72      ", DCC), 
                  Train( 2  , "nr2       ", DCC), 
                  Train( 4  , "nr4       ", DCC), 
                  Train( 24 , "Electric  ", UNCONF), //0220
                  Train( 60 , "Railcar   ", UNCONF),  //0202
                  Train( 72 , "Diesel    ", MOTOROLA),   //0022
                  Train( 78 , "Steam     ", MOTOROLA),    //0222
                  Train( 80 , "Delta Pilot", UNCONF), 
                  
                  };

//===========================================================================
//================================ Pin Outputs  =============================
//===========================================================================

#define DCC_POWER   11
#define DCC_DIR     13

//===========================================================================
//================================= IR Remote  ==============================
//===========================================================================

// Comment / Uncomment #define IRSUPPORT to enable or disable support for IR remote
#define IRSUPPORT

#define IR_PIN A3

// Look in remotes.h for remote options, or create and map your own remote like in remote_basic1.h
#define IR_REMOTE BASIC1
//#define IR_REMOTE KJELL



//===========================================================================
//============================= Short protection  ===========================
//===========================================================================

// Smaller number is faster detection bigger is more sluggish
#define SHORT_SMOOTHING 2
#define CURRENT_LIMIT 660 // this is in the 0-1023 analog value, 660 is 2A with the motor shield with 1.64v/A
#define DISPLAY_CURRENT_SMOOTHING 100 // Smoothing for the display value

