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

#ifndef APR_USER_H
#define APR_USER_H

#include "apr.h"
#include "apr_errno.h"
#include "apr_pools.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @package APR user id services
 */

/**
 * Structure for determining user ownership.
 * @defvar apr_uid_t
 */
#ifdef WIN32
typedef PSID                      apr_uid_t;
#else
typedef uid_t                     apr_uid_t;
#endif

/**
 * Structure for determining group ownership.
 * @defvar apr_gid_t
 */
#ifdef WIN32
typedef PSID                      apr_gid_t;
#else
typedef gid_t                     apr_gid_t;
#endif

#if APR_HAS_USER

/***
 * Get the user name for a specified userid
 * @param dirname Pointer to new string containing user name (on output)
 * @param userid The userid
 * @param p The pool from which to allocate the string
 * @deffunc apr_status_t apr_get_username(char **username, apr_uid_t userid, apr_pool_t *p)
 * @tip This function is available only if APR_HAS_USER is defined.
 */
APR_DECLARE(apr_status_t) apr_get_username(char **username, apr_uid_t userid, apr_pool_t *p);

/***
 * Get the home directory for the named user
 * @param dirname Pointer to new string containing directory name (on output)
 * @param userid The named user
 * @param p The pool from which to allocate the string
 * @deffunc apr_status_t apr_get_home_directory(char **dirname, const char *userid, apr_pool_t *p)
 * @tip This function is available only if APR_HAS_USER is defined.
 */
APR_DECLARE(apr_status_t) apr_get_home_directory(char **dirname, const char *userid, apr_pool_t *p);

/***
 * Get the group name for a specified groupid
 * @param dirname Pointer to new string containing group name (on output)
 * @param userid The groupid
 * @param p The pool from which to allocate the string
 * @deffunc apr_status_t apr_get_groupname(char **groupname, apr_gid_t userid, apr_pool_t *p)
 * @tip This function is available only if APR_HAS_USER is defined.
 */
APR_DECLARE(apr_status_t) apr_get_groupname(char **groupname, apr_gid_t groupid, apr_pool_t *p)

#endif  /* ! APR_HAS_USER */

#ifdef __cplusplus
}
#endif

#endif  /* ! APR_USER_H */
