/* -*-mode:java; c-basic-offset:2; -*- */
/*
Copyright (c) 2002,2003,2004 ymnk, JCraft,Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright 
     notice, this list of conditions and the following disclaimer in 
     the documentation and/or other materials provided with the distribution.

  3. The names of the authors may not be used to endorse or promote products
     derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL JCRAFT,
INC. OR ANY CONTRIBUTORS TO THIS SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

package com.jcraft.jsch;

public class JSchJ2MECLDC extends JSch{
  public JSchJ2MECLDC(){
    super();
    try{
//      Class c=Class.forName("org.bouncycastle.LICENSE");
      java.util.Hashtable config=new java.util.Hashtable();
      config.put("dh",            "com.jcraft.jsch.bc.DH");
      config.put("blowfish-cbc",  "com.jcraft.jsch.bc.BlowfishCBC");
      config.put("3des-cbc",      "com.jcraft.jsch.bc.TripleDESCBC");
      config.put("sha-1",         "com.jcraft.jsch.bc.SHA1");
      config.put("md5",           "com.jcraft.jsch.bc.MD5");
      config.put("hmac-md5",      "com.jcraft.jsch.bc.HMACMD5");
      config.put("hmac-md5-96",   "com.jcraft.jsch.bc.HMACMD596");
      config.put("hmac-sha1",     "com.jcraft.jsch.bc.HMACSHA1");
      config.put("hmac-sha1-96",  "com.jcraft.jsch.bc.HMACSHA196");
      config.put("signature.dss", "com.jcraft.jsch.bc.SignatureDSA");
      config.put("signature.rsa", "com.jcraft.jsch.bc.SignatureRSA");
      config.put("keypairgen.dsa","com.jcraft.jsch.bc.KeyPairGenDSA");
      config.put("keypairgen.rsa","com.jcraft.jsch.bc.KeyPairGenRSA");
      config.put("random",        "com.jcraft.jsch.bc.Random");
      setConfig(config);
    }
    catch(Exception e){
    }
  }
}
