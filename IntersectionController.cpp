#include "IntersectionController.h"
#include "RemoteController.h"

using namespace ControllerInfo;

/*constructor*/
IntersectionController::IntersectionController(unsigned int type,
    const std::string &centralName, const std::string &intersectionName) 
{
    this->type = type;
    strcpy(this->centralName, centralName.c_str());
    strcpy(this->intersectionName, intersectionName.c_str());

    /*create state map*/
    this->initialiseStates();

    inQueue = new Queue((char *)this->intersectionName, this);

    this->initClock();
}

/*used when a message is received by the queue thread, sets or clears the flag
specified to achieve the expected result*/
void IntersectionController::receiveMessage(char *sender, int header, int msg)
{
    if (header == SET_CONTROLLER_FLAG)
        this->setFlag(msg);
    else if (header == CLEAR_CONTROLLER_FLAG)
        this->clearFlag(msg);
}

/*called each second, when the timer runs out this calls the state transition*/
void IntersectionController::trigger()
{
    if (this->getTime() > 0)
        return;

    if ((this->stateRecord[this->state]) != NULL)
        (this->*stateRecord[this->state])();
    else
        (this->*stateRecord[STARTUP])();
}

/*sets the flag specified, called when a message is received or a sensor is
triggered*/
void IntersectionController::setFlag(unsigned int flag)
{
    /*TODO: Semaphore*/
    if (flag < CONTROLLER_COMMAND_SENTINEL)
        /*regular case - just set the flag*/
        this->flags[flag % CONTROLLER_FLAG_SENTINEL] = 1;
    else if (flag < CONTROLLER_FLAG_SENTINEL)
        /*special case for commands - hold only one integer value*/
        this->flags[SYSTEM_COMMAND] = flag;
    else
        /*special case for modes - hold only one integer value*/
        this->flags[SYSTEM_MODE] = flag;
}

/*accessor for flag, multiple cases as flag is stored in bit string*/
int IntersectionController::getFlag(unsigned int flag) 
{
    //TODO: Semaphore*/
    if (flag < CONTROLLER_COMMAND_SENTINEL)
        /*regular case - just set the flag*/
        return this->flags[flag % CONTROLLER_FLAG_SENTINEL];
    else if (flag < CONTROLLER_FLAG_SENTINEL)
        /*special case for commands - hold only one integer value*/
        return (this->flags[SYSTEM_COMMAND] == flag);
    else
        /*special case for modes - hold only one integer value*/
        return (this->flags[SYSTEM_MODE] == flag);
}

/*clears a flag, used when exiting a state in sensor mode or when exiting
command mode*/
void IntersectionController::clearFlag(unsigned int flag) {
    /*TODO: Semaphore*/
    this->flags[flag % CONTROLLER_FLAG_SENTINEL] = 0;
}

/*go to initial state at startup*/
void IntersectionController::startup()
{
    this->transitionToState(NS_CLEAR, T_NS_CLEAR);
}

void IntersectionController::ns_clear()
{
    if (
            //NS_TRAM_G guards
            (this->getFlag(TIMER_MODE) && this->getFlag(SEQ_TRAM)) ||
            (this->getFlag(COMMAND_MODE) && this->getFlag(CMD_TRAM)) ||
            (this->getFlag(SENSOR_MODE) && this->getFlag(SEN_TRAM))
       )
        this->transitionToState(NS_TRAM_G, T_TRAM_G);

    else if (
            //NS_STRAIGHT guards
            this->getFlag(SENSOR_MODE) ||
            (this->getFlag(COMMAND_MODE) && this->getFlag(CMD_NS_PED)) ||
            (this->getFlag(COMMAND_MODE) && this->getFlag(CMD_NS_STRAIGHT)) ||
            (this->getFlag(TIMER_MODE) && this->getFlag(SEQ_NS_STRAIGHT)) ||
            (this->getFlag(TIMER_MODE) && this->getFlag(SEQ_NS_PED))
            )

        this->transitionToState(NS_STRAIGHT, T_NS_STRAIGHT);

    else if (
            //EW_CLEAR guards
            (this->getFlag(COMMAND_MODE) && this->getFlag(CMD_EW_STRAIGHT)) || 
            (this->getFlag(COMMAND_MODE) && this->getFlag(CMD_EW_PED)) || 
            (this->getFlag(COMMAND_MODE) && this->getFlag(CMD_EW_RIGHT))
            )

        this->transitionToState(EW_CLEAR, T_EW_CLEAR);

    else
        //no transition defined - crash
        this->transitionToState(STARTUP, T_STARTUP);
}

void IntersectionController::ns_tram_g()
{
    if (
       //loop condition
       this->getFlag(COMMAND_MODE) && this->getFlag(CMD_TRAM)
       )
        this->transitionToState(NS_TRAM_G, T_TRAM_G);
    this->transitionToState(NS_TRAM_F, T_TRAM_F);
}

void IntersectionController::ns_tram_f()
{
    //note - previously went back to ns clear, loops permanently in timer mode
    this->transitionToState(NS_STRAIGHT, T_NS_CLEAR);
}

void IntersectionController::ns_straight()
{
    if (
            //NS_STRAIGHT_G_PED_G guards
            (this->getFlag(TIMER_MODE) && this->getFlag(SEQ_NS_PED)) ||
            (this->getFlag(SENSOR_MODE) && this->getFlag(SEN_NS_PED)) ||
            (this->getFlag(COMMAND_MODE) && this->getFlag(CMD_NS_PED))
            )
        this->transitionToState(NS_STRAIGHT_G_PED_G, T_NS_PED_G);

    else if (
       //NS_STRAIGHT_G guards
       this->getFlag(COMMAND_MODE) ||
       this->getFlag(TIMER_MODE) ||
       this->getFlag(SENSOR_MODE)
       )
        this->transitionToState(NS_STRAIGHT_G, T_NS_STRAIGHT_G);

    else
        //crash
        this->transitionToState(STARTUP, T_STARTUP);
}

void IntersectionController::ns_straight_g_ped_g()
{
    if (
       //loop
       this->getFlag(COMMAND_MODE) && this->getFlag(CMD_NS_PED)
       )
        this->transitionToState(NS_STRAIGHT_G_PED_G, T_NS_STRAIGHT_CHECK);

    else if (
       //NS_STRAIGHT_G_PED_F guards
       this->getFlag(SENSOR_MODE) ||
       this->getFlag(TIMER_MODE)
       )
        this->transitionToState(NS_STRAIGHT_G_PED_F, T_NS_PED_F);

    else
        //crash
        this->transitionToState(STARTUP, T_STARTUP);
}

void IntersectionController::ns_straight_g_ped_f()
{
    this->transitionToState(NS_STRAIGHT_G, T_NS_STRAIGHT_G_QUICK);
}

void IntersectionController::ns_straight_g()
{
    if (
       //NS_STRAIGHT_G guards
       this->getFlag(COMMAND_MODE) && this->getFlag(CMD_NS_STRAIGHT)
       )
        this->transitionToState(NS_STRAIGHT_G, T_NS_STRAIGHT_G_QUICK);
    else
        this->transitionToState(NS_STRAIGHT_F, T_NS_STRAIGHT_F);
}

void IntersectionController::ns_straight_f()
{
    this->transitionToState(EW_CLEAR, T_EW_CLEAR);
}

void IntersectionController::ew_clear()
{
    if (
       //EW_BOTH_RIGHT guards
       (this->getFlag(TIMER_MODE) && this->getFlag(SEQ_EW_RIGHT)) ||
       (this->getFlag(SENSOR_MODE) && this->getFlag(SEN_EW_RIGHT)) ||
       (this->getFlag(COMMAND_MODE) && this->getFlag(CMD_EW_RIGHT))
       )
        this->transitionToState(EW_BOTH_RIGHT_G, T_EW_BOTH_RIGHT_G);

    else if (
       //EW_STRAIGHT guards
       (this->getFlag(TIMER_MODE) && !this->getFlag(SEQ_EW_RIGHT)) ||
       (this->getFlag(SENSOR_MODE) && !this->getFlag(SEN_EW_RIGHT)) ||
       (this->getFlag(COMMAND_MODE) && this->getFlag(CMD_EW_STRAIGHT)) ||
       this->getFlag(CMD_EW_PED)
       )
        this->transitionToState(EW_STRAIGHT, T_EW_STRAIGHT);
    else if (
       //NS_CLEAR guards
       (this->getFlag(COMMAND_MODE) && !this->getFlag(CMD_NS_PED)) ||
       (this->getFlag(COMMAND_MODE) && !this->getFlag(CMD_NS_STRAIGHT)) ||
       (this->getFlag(COMMAND_MODE) && this->getFlag(CMD_TRAM))
       )
        this->transitionToState(NS_CLEAR, T_NS_CLEAR);
    else
        //crash
        this->transitionToState(STARTUP, T_STARTUP);
}

void IntersectionController::ew_both_right_g()
{
    if (
       //loop
       this->getFlag(COMMAND_MODE) && this->getFlag(CMD_EW_RIGHT)
       )
        this->transitionToState(EW_BOTH_RIGHT_G, T_EW_BOTH_RIGHT_G);

    else if (
       //EW_BOTH_RIGHT_F guards
       this->getFlag(SENSOR_MODE) ||
       this->getFlag(TIMER_MODE) ||
       (this->getFlag(COMMAND_MODE) && !this->getFlag(CMD_EW_RIGHT))
       )
        this->transitionToState(EW_BOTH_RIGHT_F, T_EW_BOTH_RIGHT_F);

    else
        //crash
        this->transitionToState(STARTUP, T_STARTUP);
}

void IntersectionController::ew_both_right_f()
{
    this->transitionToState(EW_STRAIGHT, T_EW_STRAIGHT);
}

void IntersectionController::ew_straight()
{
    if (
       //EW_STRAIGHT_G_PED_G guards
       (this->getFlag(SENSOR_MODE) && this->getFlag(SEN_EW_PED))||
       (this->getFlag(TIMER_MODE) && this->getFlag(SEQ_EW_PED)) || 
       (this->getFlag(COMMAND_MODE) && !this->getFlag(CMD_EW_PED))
       )
        this->transitionToState(EW_STRAIGHT_G_PED_G, T_EW_PED_G);

    else if (
       //EW_STRAIGHT_G guards
       this->getFlag(COMMAND_MODE) ||
       this->getFlag(TIMER_MODE)  ||
       this->getFlag(SENSOR_MODE)
       )
        this->transitionToState(EW_STRAIGHT_G, T_EW_STRAIGHT_G);
    else
        //crash
        this->transitionToState(STARTUP, T_STARTUP);
}

void IntersectionController::ew_straight_g_ped_g()
{
    if (
       //loop
       (this->getFlag(COMMAND_MODE) && this->getFlag(CMD_EW_PED))
       )
        this->transitionToState(EW_STRAIGHT_G_PED_G, T_EW_PED_G);

    else if (
       //EW_STRAIGHT_G_PED_F guards
       this->getFlag(SENSOR_MODE) ||
       this->getFlag(TIMER_MODE) ||
       this->getFlag(COMMAND_MODE)
       )
        this->transitionToState(EW_STRAIGHT_G_PED_F, T_EW_PED_F);
    else
        //crash
        this->transitionToState(STARTUP, T_STARTUP);
}

void IntersectionController::ew_straight_g_ped_f()
{
    this->transitionToState(EW_STRAIGHT_G, T_EW_STRAIGHT_G_QUICK);
}

void IntersectionController::ew_straight_g()
{
    if (
       //EW_STRAIGHT_G_PED_G guards
       this->getFlag(SENSOR_MODE) && this->getFlag(SEN_EW_PED)
       )
        this->transitionToState(EW_STRAIGHT_G_PED_G, T_EW_PED_G);

    else if (
       //EW_STRAIGHT_F guards
       this->getFlag(TIMER_MODE) ||
       this->getFlag(COMMAND_MODE) ||
       (this->getFlag(SENSOR_MODE) && this->getFlag(SEN_NS_STRAIGHT)) ||
       (this->getFlag(SENSOR_MODE) && this->getFlag(SEN_NS_PED)) ||
       (this->getFlag(SENSOR_MODE) && this->getFlag(SEN_TRAM))
       )
        this->transitionToState(EW_STRAIGHT_F, T_EW_STRAIGHT_F);

    else if (
       //loop
       (this->getFlag(COMMAND_MODE) && this->getFlag(CMD_EW_STRAIGHT)) ||
       this->getFlag(SENSOR_MODE)
       )
        this->transitionToState(EW_STRAIGHT_G, T_EW_STRAIGHT_CHECK);

    else
        //crash
        this->transitionToState(STARTUP, T_STARTUP);
}

void IntersectionController::ew_straight_f()
{
    this->transitionToState(NS_CLEAR, T_NS_CLEAR);
}

/*stores the state in the table when the intersection is created*/
void IntersectionController::
mapState(controllerState state,
         void (IntersectionController::*stateFunction)(), 
         lightString lightFlagsNS, lightString lightFlagsEW,
         lightString flashFlagsNS, lightString flashFlagsEW,
         int clearFlags
        )
{
    this->stateRecord[state] = stateFunction;
    
    this->lightFlagsNS[state] = lightFlagsNS;
    this->lightFlagsEW[state] = lightFlagsEW;

    this->flashFlagsNS[state] = flashFlagsNS;
    this->flashFlagsEW[state] = flashFlagsEW;

    this->exitClearFlags[state] = clearFlags;
}

/*display the intersection in ascii art*/
void IntersectionController::display()
{
    /*north-south light states*/
    unsigned char ns_g, ns_a, ns_r;
    unsigned char ns_t_w, ns_t_a;
    unsigned char ns_p_g, ns_p_r;

    lightsNS->getLights() & (1 << CAR_STRAIGHT_GO) ? ns_g = 'G' : ns_g = ' ';
    lightsNS->getLights() & (1 << CAR_STRAIGHT_FINISH) ? ns_a = 'A' : ns_a = ' ';
    lightsNS->getLights() & (1 << CAR_STRAIGHT_STOP) ? ns_r = 'R' : ns_r = ' ';

    lightsNS->getLights() & (1 << TRAM_GO) ? ns_t_w = 'W' : ns_t_w = ' ';
    lightsNS->getLights() & (1 << TRAM_FINISH) ? ns_t_a = 'A' : ns_t_a = ' ';

    lightsNS->getLights() & (1 << PEDESTRIAN_GO) ? ns_p_g = 'G' : ns_p_g = ' ';
    lightsNS->getLights() & (1 << PEDESTRIAN_STOP) ? ns_p_r = 'R' : ns_p_r = ' ';

    /*north-south sensor states*/
    unsigned char ns, ns_t, ns_p;

    flags[SEN_NS_STRAIGHT] ? ns = 'X' : ns = ' ';
    flags[SEN_TRAM] ? ns_t = 'X' : ns_t = ' ';
    flags[SEN_NS_PED] ? ns_p = 'X' : ns_p = ' ';

    /*east-west light states*/
    unsigned char ew_g, ew_a, ew_r;
    unsigned char ew_t_g, ew_t_a;
    unsigned char ew_p_g, ew_p_r;

    lightsEW->getLights() & (1 << CAR_STRAIGHT_GO) ? ew_g = 'G' : ew_g = ' ';
    lightsEW->getLights() & (1 << CAR_STRAIGHT_FINISH) ? ew_a = 'A' :
        ew_a = ' ';
    lightsEW->getLights() & (1 << CAR_STRAIGHT_STOP) ? ew_r = 'R' : ew_r = ' ';

    lightsEW->getLights() & (1 << CAR_RIGHT_GO) ? ew_t_g = 'G' : ew_t_g = ' ';
    lightsEW->getLights() & (1 << CAR_RIGHT_FINISH) ? ew_t_a = 'A' :
        ew_t_a = ' ';

    lightsEW->getLights() & (1 << PEDESTRIAN_GO) ? ew_p_g = 'G' : ew_p_g = ' ';
    lightsEW->getLights() & (1 << PEDESTRIAN_STOP) ? ew_p_r = 'R' :
        ew_p_r = ' ';

    /*east-west sensor states*/
    unsigned char ew, ew_t, ew_p;

    flags[SEN_EW_STRAIGHT] ? ew = 'X' : ew = ' ';
    flags[SEN_EW_RIGHT] ? ew_t = 'X' : ew_t = ' ';
    flags[SEN_EW_PED] ? ew_p = 'X' : ew_p = ' ';

    /*output*/
    printf(" ----------------------- \n");
    printf("|       | %c %c %c |       |\n", ns, ns_t, ns_p);
    printf("|       | - - - |       |\n");
    printf("|       ||%c|%c|%c||       |\n", ns_g, ns_t_w, ns_p_g);
    printf("|       ||%c|%c|%c||       |\n", ns_a, ns_t_a, ns_p_r);
    printf("|       ||%c|- - |       |\n", ns_r);
    printf("|       | -     |       |\n");
    printf("|-----------------------|\n");
    printf("|  --   |       |  ---  |\n");
    printf("|%c|%c%c|  |       | |%c%c%c|%c|\n", ew_p, ew_p_g, ew_p_r, ew_r,
        ew_a, ew_g, ew);
    printf("|  --   |       |  ---  |\n");
    printf("|%c|%c%c|  |       |  |%c%c|%c|\n", ew_t, ew_t_a, ew_t_g, ew_t_g,
        ew_t_a, ew_t);
    printf("|  ---  |       |   --  |\n");
    printf("|%c|%c%c%c| |       |  |%c%c|%c|\n", ew, ew_g, ew_a, ew_r, ew_p_r,
        ew_p_g, ew_p);
    printf("|  ---  |       |   --  |\n");
    printf("|-----------------------|\n");
    printf("|       |     - |       |\n");
    printf("|       | - -|%c||       |\n", ns_r);
    printf("|       ||%c|%c|%c||       |\n", ns_p_r, ns_t_a, ns_a);
    printf("|       ||%c|%c|%c||       |\n", ns_p_g, ns_t_w, ns_g);
    printf("|       | - - - |       |\n");
    printf("|       | %c %c %c |       |\n", ns_p, ns_t, ns);
    printf(" ----------------------- \n");
}

/*move to new state, uses function pointer array and enums*/
void IntersectionController::transitionToState(controllerState state, int time)
{
    /*clear any relevant flags before exiting current state*/
    this->clearFlags(this->exitClearFlags[this->state]);

    /*set lights*/
    this->lightsNS->setState(this->lightFlagsNS[state], 
                             this->flashFlagsNS[state]);

    this->lightsEW->setState(this->lightFlagsEW[state],
                             this->flashFlagsEW[state]);

    this->resetTimer(time);
    this->state = state;

    std::cout << "Transition to state: " << controllerStateNames[state] << "\n";

    char *dest = (char *)this->centralName;
    char *noString = (char *)"";

    /*build a bitstring from our flags*/
    int flags = 0;
    for (int i = 0; i < CONTROLLER_MODE_SENTINEL; i++)
        if (this->flags[i])
            flags &= (1 << i);

    /*notify central controller*/
    write_queue(dest, RemoteInfo::NOTIFY_STATE, noString, (int)state);
    write_queue(dest, RemoteInfo::NOTIFY_FLAGS, noString, flags);
}

/*clear all mode flags*/
void IntersectionController::clearFlags(int bitString)
{
    for (unsigned int i = 0; i < CONTROLLER_FLAG_SENTINEL; i++)
        if (1 & (bitString >> i))
        {
            this->clearFlag(i);
            std::cerr << "Clearing " << controllerFlagNames[i] << 
                         " (" << i << ")\n";
        }
}

/*initialise the intersection states, set the function pointers for each valid
state*/
void IntersectionController::initialiseStates() 
{
    std::cerr << "initialiseStates\n";

    /*clear all flags */
    this->clearFlags(0xFFFF);

    /*populate state maps*/
    mapState(STARTUP, &IntersectionController::startup, 
             STARTUP_L_NS, STARTUP_L_EW, 
             STARTUP_F_NS, STARTUP_F_EW, 
             STARTUP_C_EXIT);

    mapState(NS_CLEAR, &IntersectionController::ns_clear, 
             NS_CLEAR_L_NS, NS_CLEAR_L_EW, 
             NS_CLEAR_F_NS, NS_CLEAR_F_EW, 
             NS_CLEAR_C_EXIT);

    if (type == TRAM)
    {
        mapState(NS_TRAM_G, &IntersectionController::ns_straight,
                 NS_TRAM_G_L_NS, NS_TRAM_G_L_EW,
                 NS_TRAM_G_F_NS, NS_TRAM_G_F_EW, 
                 NS_TRAM_G_C_EXIT);

        mapState(NS_TRAM_F, &IntersectionController::ns_tram_f,
                 NS_TRAM_F_L_NS, NS_TRAM_F_L_EW,
                 NS_TRAM_F_F_NS, NS_TRAM_F_F_EW, 
                 NS_TRAM_F_C_EXIT);
    }
    else
    {
        //map tram to NS_STRAIGHT if this is not a tram intersection
        mapState(NS_TRAM_G, &IntersectionController::ns_straight,
                 NS_STRAIGHT_L_NS, NS_STRAIGHT_L_EW,
                 NS_STRAIGHT_F_NS, NS_STRAIGHT_F_EW, 
                 NS_STRAIGHT_C_EXIT);

        mapState(NS_TRAM_F, &IntersectionController::ns_straight,
                 NS_STRAIGHT_L_NS, NS_STRAIGHT_L_EW,
                 NS_STRAIGHT_F_NS, NS_STRAIGHT_F_EW, 
                 NS_STRAIGHT_C_EXIT);
    }

    mapState(NS_STRAIGHT, &IntersectionController::ns_straight,
             NS_STRAIGHT_L_NS, NS_STRAIGHT_L_EW,
             NS_STRAIGHT_F_NS, NS_STRAIGHT_F_EW, 
             NS_STRAIGHT_C_EXIT);

    mapState(NS_STRAIGHT_G_PED_G, &IntersectionController::ns_straight_g_ped_g,
             NS_STRAIGHT_G_PED_G_L_NS, NS_STRAIGHT_G_PED_G_L_EW,
             NS_STRAIGHT_G_PED_G_F_NS, NS_STRAIGHT_G_PED_G_F_EW, 
             NS_STRAIGHT_G_PED_G_C_EXIT);

    mapState(NS_STRAIGHT_G_PED_F, &IntersectionController::ns_straight_g_ped_f,
             NS_STRAIGHT_G_PED_F_L_NS, NS_STRAIGHT_G_PED_F_L_EW,
             NS_STRAIGHT_G_PED_F_F_NS, NS_STRAIGHT_G_PED_F_F_EW, 
             NS_STRAIGHT_G_PED_F_C_EXIT);

    mapState(NS_STRAIGHT_G_PED_F, &IntersectionController::ns_straight_g_ped_f,
             NS_STRAIGHT_G_PED_F_L_NS, NS_STRAIGHT_G_PED_F_L_EW,
             NS_STRAIGHT_G_PED_F_F_NS, NS_STRAIGHT_G_PED_F_F_EW, 
             NS_STRAIGHT_G_PED_F_C_EXIT);

    mapState(NS_STRAIGHT_G, &IntersectionController::ns_straight_g,
             NS_STRAIGHT_G_L_NS, NS_STRAIGHT_G_L_EW,
             NS_STRAIGHT_G_F_NS, NS_STRAIGHT_G_F_EW, 
             NS_STRAIGHT_G_C_EXIT);

    mapState(NS_STRAIGHT_F, &IntersectionController::ns_straight_f,
             NS_STRAIGHT_F_L_NS, NS_STRAIGHT_F_L_EW,
             NS_STRAIGHT_F_F_NS, NS_STRAIGHT_F_F_EW, 
             NS_STRAIGHT_F_C_EXIT);

    mapState(EW_CLEAR, &IntersectionController::ew_clear,
             EW_CLEAR_L_NS, EW_CLEAR_L_EW,
             EW_CLEAR_F_NS, EW_CLEAR_F_EW, 
             EW_CLEAR_C_EXIT);

    mapState(EW_BOTH_RIGHT_G, &IntersectionController::ew_both_right_g,
             EW_BOTH_RIGHT_G_L_NS, EW_BOTH_RIGHT_G_L_EW,
             EW_BOTH_RIGHT_G_F_NS, EW_BOTH_RIGHT_G_F_EW, 
             EW_BOTH_RIGHT_G_C_EXIT);

    mapState(EW_BOTH_RIGHT_F, &IntersectionController::ew_both_right_f,
             EW_BOTH_RIGHT_F_L_NS, EW_BOTH_RIGHT_F_L_EW,
             EW_BOTH_RIGHT_F_F_NS, EW_BOTH_RIGHT_F_F_EW, 
             EW_BOTH_RIGHT_F_C_EXIT);

    mapState(EW_STRAIGHT, &IntersectionController::ew_straight,
             EW_STRAIGHT_L_NS, EW_STRAIGHT_L_EW,
             EW_STRAIGHT_F_NS, EW_STRAIGHT_F_EW, 
             EW_STRAIGHT_C_EXIT);

    mapState(EW_STRAIGHT_G_PED_G, &IntersectionController::ew_straight_g_ped_g,
             EW_STRAIGHT_G_PED_G_L_NS, EW_STRAIGHT_G_PED_G_L_EW,
             EW_STRAIGHT_G_PED_G_F_NS, EW_STRAIGHT_G_PED_G_F_EW, 
             EW_STRAIGHT_G_PED_G_C_EXIT);

    mapState(EW_STRAIGHT_G_PED_F, &IntersectionController::ew_straight_g_ped_f,
             EW_STRAIGHT_G_PED_F_L_NS, EW_STRAIGHT_G_PED_F_L_EW,
             EW_STRAIGHT_G_PED_F_F_NS, EW_STRAIGHT_G_PED_F_F_EW, 
             EW_STRAIGHT_G_PED_F_C_EXIT);

    mapState(EW_STRAIGHT_G, &IntersectionController::ew_straight_g,
             EW_STRAIGHT_G_L_NS, EW_STRAIGHT_G_L_EW,
             EW_STRAIGHT_G_F_NS, EW_STRAIGHT_G_F_EW, 
             EW_STRAIGHT_G_C_EXIT);

    mapState(EW_STRAIGHT_F, &IntersectionController::ew_straight_f,
             EW_STRAIGHT_F_L_NS, EW_STRAIGHT_F_L_EW,
             EW_STRAIGHT_F_F_NS, EW_STRAIGHT_F_F_EW, 
             EW_STRAIGHT_F_C_EXIT);

    /*add some lights*/
    lightString lightConf;
    lightConf = (NOTRAM == this->type ?  I1_I3_NS_LIGHT_CONF :
        I2_NS_LIGHT_CONF);
    this->lightsNS = new LightHandler(lightConf);

    lightConf = (NOTRAM == this->type ?  I1_I3_EW_LIGHT_CONF :
        I2_EW_LIGHT_CONF);
    this->lightsEW = new LightHandler(lightConf);

    /*default to sequence mode*/
    this->setFlag(TIMER_MODE);

    /*set default sequence, determined by order set by the setFlag function*/
    if (type == TRAM)
        this->setFlag(SEQ_TRAM);
    this->setFlag(SEQ_NS_STRAIGHT);
    this->setFlag(SEQ_NS_PED);

    this->setFlag(SEQ_EW_RIGHT);
    this->setFlag(SEQ_EW_STRAIGHT);
    this->setFlag(SEQ_EW_PED);

    /*initial state*/
    this->transitionToState(STARTUP, T_STARTUP);
}

