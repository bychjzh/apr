/* ====================================================================
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Apache" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

#include "threadproc.h"
#include "apr_thread_proc.h"
#include "apr_general.h"
#include "apr_lib.h"
#include "fileio.h"
#include <stdlib.h>
#define INCL_DOS
#include <os2.h>

ap_status_t ap_create_threadattr(ap_threadattr_t **new, ap_context_t *cont)
{
    (*new) = (ap_threadattr_t *)ap_palloc(cont, sizeof(ap_threadattr_t));

    if ((*new) == NULL) {
        return APR_ENOMEM;
    }

    (*new)->cntxt = cont;
    (*new)->attr = 0;
    return APR_SUCCESS;
}



ap_status_t ap_setthreadattr_detach(ap_threadattr_t *attr, ap_int32_t on)
{
    attr->attr |= APR_THREADATTR_DETACHED;
    return APR_SUCCESS;
}



ap_status_t ap_getthreadattr_detach(ap_threadattr_t *attr)
{
    return (attr->attr & APR_THREADATTR_DETACHED) ? APR_DETACH : APR_NOTDETACH;
}



static void ap_thread_begin(void *arg)
{
  ap_thread_t *thread = (ap_thread_t *)arg;
  thread->rv = thread->func(thread->data);
}



ap_status_t ap_create_thread(ap_thread_t **new, ap_threadattr_t *attr, 
                             ap_thread_start_t func, void *data, 
                             ap_context_t *cont)
{
    ap_status_t stat;
    ap_thread_t *thread;
 
    thread = (ap_thread_t *)ap_palloc(cont, sizeof(ap_thread_t));
    *new = thread;

    if (thread == NULL) {
        return APR_ENOMEM;
    }

    thread->cntxt = cont;
    thread->attr = attr;
    thread->func = func;
    thread->data = data;
    stat = ap_create_context(&thread->cntxt, cont);
    
    if (stat != APR_SUCCESS) {
        return stat;
    }

    if (attr == NULL) {
        stat = ap_create_threadattr(&thread->attr, thread->cntxt);
        
        if (stat != APR_SUCCESS) {
            return stat;
        }
    }
    
    if (thread->attr->attr & APR_THREADATTR_DETACHED)
        thread->tid = _beginthread((os2_thread_start_t)func, NULL, APR_THREAD_STACKSIZE, data);
    else
        thread->tid = _beginthread(ap_thread_begin, NULL, APR_THREAD_STACKSIZE, thread);
        
    if (thread->tid < 0) {
        return errno;
    }

    return APR_SUCCESS;
}



ap_status_t ap_thread_exit(ap_thread_t *thd, ap_status_t *retval)
{
    thd->rv = retval;
    _endthread();
    return -1; /* If we get here something's wrong */
}



ap_status_t ap_thread_join(ap_status_t *retval, ap_thread_t *thd)
{
    ULONG rc;
    TID waittid = thd->tid;

    if (thd->attr->attr & APR_THREADATTR_DETACHED)
        return APR_ENOSTAT;

    rc = DosWaitThread(&waittid, DCWW_WAIT);
    *retval = (ap_status_t)thd->rv;
    return os2errno(rc);
}



ap_status_t ap_thread_detach(ap_thread_t *thd)
{
    thd->attr->attr |= APR_THREADATTR_DETACHED;
    return APR_SUCCESS;
}


