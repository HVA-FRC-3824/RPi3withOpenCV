//
//  cmdProcessor.hpp
//  Video
//
//  Created by Robert Palmer Jr on 2/6/17.
//  Copyright © 2017 Bryson Gullett. All rights reserved.
//

#ifndef cmdProcessor_hpp
#define cmdProcessor_hpp

#include <stdio.h>

class cmdProcessor
{
private:
    cmdProcessor();
    ~cmdProcessor();
    
private:
    static cmdProcessor* pInstance;
    bool serverRunning;
    
public:
    static cmdProcessor* getInstance();
    void start();
    void processMessage(char* buffer);
    bool isRunning();
    
private:
    void *get_in_addr(struct sockaddr * sa);

};
#endif /* cmdProcessor_hpp */
