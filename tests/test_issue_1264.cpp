#/*
    Copyright (c) 2007-2016 Contributors as noted in the AUTHORS file

    This file is part of libzmq, the ZeroMQ core engine in C++.

    libzmq is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    As a special exception, the Contributors give you permission to link
    this library with independent modules to produce an executable,
    regardless of the license terms of these independent modules, and to
    copy and distribute the resulting executable under terms of your choice,
    provided that you also meet, for each linked independent module, the
    terms and conditions of the license of that module. An independent
    module is a module which is not derived from or based on this library.
    If you modify this library, you must extend this exception to your
    version of the library.

    libzmq is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "testutil.hpp"

int main(void)
{
    void* ctx = zmq_ctx_new();
    assert(ctx);

    // Send message
    
    void* sck1 = zmq_socket (ctx, ZMQ_DEALER);
    assert(sck1);
    int rc = zmq_connect (sck1, "tcp://localhost:59999");
    assert(rc == 0);

    char sbuf[255];
    memset (sbuf, 'x', sizeof (sbuf));
    int len = zmq_send (sck1, sbuf, sizeof(sbuf), 0);
    assert(len >= 0);

    char quit = {'q'};
    len = zmq_send (sck1, &quit, sizeof(quit), 0);
    assert (len >= 0);

    zmq_close (sck1);
    
    // Receive messages
    void* sck2 = zmq_socket (ctx, ZMQ_DEALER);
    assert (sck2);
    rc = zmq_bind (sck2, "tcp://*:59999");
    assert (rc == 0);
    
    char rbuf[256];
    int nbytes = 0;
    
    do {
        nbytes = zmq_recv (sck2, &rbuf, 256, 0);
        printf ("nbytes = %d\n", nbytes);
        assert (nbytes >= 0 && nbytes <= 255);
        rbuf[nbytes] = '\0';
        printf ("%s\n", rbuf);
    } while (nbytes != 1);
    
    zmq_close (sck1);
    zmq_close (sck2);
    
    zmq_ctx_term (ctx);
    return 0;
}
