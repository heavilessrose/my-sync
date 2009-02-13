package org.bouncycastle.asn1.test;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.math.BigInteger;
import java.util.Date;
import java.util.Hashtable;
import java.util.Vector;

import org.bouncycastle.util.encoders.Base64;
import org.bouncycastle.util.test.Test;
import org.bouncycastle.util.test.TestResult;
import org.bouncycastle.util.test.SimpleTestResult;
import org.bouncycastle.asn1.ASN1EncodableVector;
import org.bouncycastle.asn1.ASN1InputStream;
import org.bouncycastle.asn1.ASN1OutputStream;
import org.bouncycastle.asn1.DERInteger;
import org.bouncycastle.asn1.DERNull;
import org.bouncycastle.asn1.DERObject;
import org.bouncycastle.asn1.DEROctetString;
import org.bouncycastle.asn1.DERSequence;
import org.bouncycastle.asn1.oiw.ElGamalParameter;
import org.bouncycastle.asn1.oiw.OIWObjectIdentifiers;
import org.bouncycastle.asn1.pkcs.PKCSObjectIdentifiers;
import org.bouncycastle.asn1.x509.AlgorithmIdentifier;
import org.bouncycastle.asn1.x509.AuthorityKeyIdentifier;
import org.bouncycastle.asn1.x509.GeneralName;
import org.bouncycastle.asn1.x509.GeneralNames;
import org.bouncycastle.asn1.x509.IssuingDistributionPoint;
import org.bouncycastle.asn1.x509.KeyUsage;
import org.bouncycastle.asn1.x509.RSAPublicKeyStructure;
import org.bouncycastle.asn1.x509.ReasonFlags;
import org.bouncycastle.asn1.x509.SubjectKeyIdentifier;
import org.bouncycastle.asn1.x509.SubjectPublicKeyInfo;
import org.bouncycastle.asn1.x509.TBSCertList;
import org.bouncycastle.asn1.x509.TBSCertificateStructure;
import org.bouncycastle.asn1.x509.Time;
import org.bouncycastle.asn1.x509.V1TBSCertificateGenerator;
import org.bouncycastle.asn1.x509.V2TBSCertListGenerator;
import org.bouncycastle.asn1.x509.V3TBSCertificateGenerator;
import org.bouncycastle.asn1.x509.X509Extension;
import org.bouncycastle.asn1.x509.X509Extensions;
import org.bouncycastle.asn1.x509.X509Name;

public class GenerationTest
    implements Test
{
    private byte[] v1Cert = Base64.decode(
          "MIGtAgEBMA0GCSqGSIb3DQEBBAUAMCUxCzAJBgNVBAMTAkFVMRYwFAYDVQQKEw1Cb"
        + "3VuY3kgQ2FzdGxlMB4XDTcwMDEwMTAwMDAwMVoXDTcwMDEwMTAwMDAxMlowNjELMA"
        + "kGA1UEAxMCQVUxFjAUBgNVBAoTDUJvdW5jeSBDYXN0bGUxDzANBgNVBAsTBlRlc3Q"
        + "gMTAaMA0GCSqGSIb3DQEBAQUAAwkAMAYCAQECAQI=");

    private byte[] v3Cert = Base64.decode(
          "MIIBSKADAgECAgECMA0GCSqGSIb3DQEBBAUAMCUxCzAJBgNVBAMTAkFVMRYwFAYD"
        + "VQQKEw1Cb3VuY3kgQ2FzdGxlMB4XDTcwMDEwMTAwMDAwMVoXDTcwMDEwMTAwMDAw"
        + "MlowNjELMAkGA1UEAxMCQVUxFjAUBgNVBAoTDUJvdW5jeSBDYXN0bGUxDzANBgNV"
        + "BAsTBlRlc3QgMjAYMBAGBisOBwIBATAGAgEBAgECAwQAAgEDo4GVMIGSMGEGA1Ud"
        + "IwEB/wRXMFWAFDZPdpHPzKi7o8EJokkQU2uqCHRRoTqkODA2MQswCQYDVQQDEwJB"
        + "VTEWMBQGA1UEChMNQm91bmN5IENhc3RsZTEPMA0GA1UECxMGVGVzdCAyggECMCAG"
        + "A1UdDgEB/wQWBBQ2T3aRz8you6PBCaJJEFNrqgh0UTALBgNVHQ8EBAMCBBA=");

    private byte[] v2CertList = Base64.decode(
          "MIIBRQIBATANBgkqhkiG9w0BAQUFADAlMQswCQYDVQQDEwJBVTEWMBQGA1UEChMN"
        + "Qm91bmN5IENhc3RsZRcNNzAwMTAxMDAwMDAwWhcNNzAwMTAxMDAwMDAyWjAkMCIC"
        + "AQEXDTcwMDEwMTAwMDAwMVowDjAMBgNVHRUEBQoDAIAAoIHFMIHCMGEGA1UdIwEB"
        + "/wRXMFWAFDZPdpHPzKi7o8EJokkQU2uqCHRRoTqkODA2MQswCQYDVQQDEwJBVTEW"
        + "MBQGA1UEChMNQm91bmN5IENhc3RsZTEPMA0GA1UECxMGVGVzdCAyggECMEMGA1Ud"
        + "EgQ8MDqkODA2MQswCQYDVQQDEwJBVTEWMBQGA1UEChMNQm91bmN5IENhc3RsZTEP"
        + "MA0GA1UECxMGVGVzdCAzMAoGA1UdFAQDAgEBMAwGA1UdHAEB/wQCMAA=");
    
    private boolean isSameAs(
            byte[]  a,
            byte[]  b)
    {
        if (a.length != b.length)
        {
            return false;
        }
        
        for (int i = 0; i != a.length; i++)
        {
            if (a[i] != b[i])
            {
                return false;
            }
        }
        
        return true;
    }
    
    private TestResult tbsV1CertGen()
    {
        try
        {
            V1TBSCertificateGenerator   gen = new V1TBSCertificateGenerator();
            Date                        startDate = new Date(1000);
            Date                        endDate = new Date(12000);
            
            gen.setSerialNumber(new DERInteger(1));
            
            gen.setStartDate(new Time(startDate));
            gen.setEndDate(new Time(endDate));
            
            gen.setIssuer(new X509Name("CN=AU,O=Bouncy Castle"));
            gen.setSubject(new X509Name("CN=AU,O=Bouncy Castle,OU=Test 1"));
            
            gen.setSignature(new AlgorithmIdentifier(PKCSObjectIdentifiers.md5WithRSAEncryption, new DERNull()));
            
            SubjectPublicKeyInfo    info = new SubjectPublicKeyInfo(new AlgorithmIdentifier(PKCSObjectIdentifiers.rsaEncryption, new DERNull()), 
                                                         new RSAPublicKeyStructure(BigInteger.valueOf(1), BigInteger.valueOf(2)));
            
            gen.setSubjectPublicKeyInfo(info);
            
            TBSCertificateStructure     tbs = gen.generateTBSCertificate();
            ByteArrayOutputStream       bOut = new ByteArrayOutputStream();
            ASN1OutputStream            aOut = new ASN1OutputStream(bOut);
            
            aOut.writeObject(tbs);
            
            if (!isSameAs(bOut.toByteArray(), v1Cert))
            {
                return new SimpleTestResult(false, getName() + ": failed v1 cert generation");
            }
            
            //
            // read back test
            //
            ASN1InputStream aIn = new ASN1InputStream(new ByteArrayInputStream(v2CertList));
            DERObject       o = aIn.readObject();
            
            bOut = new ByteArrayOutputStream();
            aOut = new ASN1OutputStream(bOut);
            
            aOut.writeObject(o);
            
            if (!isSameAs(bOut.toByteArray(), v2CertList))
            {
                return new SimpleTestResult(false, getName() + ": failed v1 cert read back test");
            }
            
            return new SimpleTestResult(true, getName() + ": Okay");
        }
        catch (Exception e)
        {
            return new SimpleTestResult(false, getName() + ": v1 cert list failed " + e.toString(), e);
        }
    }
    
    private AuthorityKeyIdentifier createAuthorityKeyId(
        SubjectPublicKeyInfo    info,
        X509Name                name,
        int                     sNumber)
    {
        GeneralName             genName = new GeneralName(name);
        ASN1EncodableVector     v = new ASN1EncodableVector();

        v.add(genName);

        return new AuthorityKeyIdentifier(
            info, new GeneralNames(new DERSequence(v)), BigInteger.valueOf(sNumber));
    }
    
    private TestResult tbsV3CertGen()
    {
        try
        {
            V3TBSCertificateGenerator   gen = new V3TBSCertificateGenerator();
            Date                        startDate = new Date(1000);
            Date                        endDate = new Date(2000);

            gen.setSerialNumber(new DERInteger(2));
            
            gen.setStartDate(new Time(startDate));
            gen.setEndDate(new Time(endDate));
            
            gen.setIssuer(new X509Name("CN=AU,O=Bouncy Castle"));
            gen.setSubject(new X509Name("CN=AU,O=Bouncy Castle,OU=Test 2"));
            
            gen.setSignature(new AlgorithmIdentifier(PKCSObjectIdentifiers.md5WithRSAEncryption, new DERNull()));
           
            SubjectPublicKeyInfo    info = new SubjectPublicKeyInfo(new AlgorithmIdentifier(OIWObjectIdentifiers.elGamalAlgorithm, new ElGamalParameter(BigInteger.valueOf(1), BigInteger.valueOf(2))), new DERInteger(3));

            gen.setSubjectPublicKeyInfo(info);
            
            //
            // add extensions
            //
            Vector          order = new Vector();
            Hashtable       extensions = new Hashtable();
            
            order.addElement(X509Extensions.AuthorityKeyIdentifier);
            order.addElement(X509Extensions.SubjectKeyIdentifier);
            order.addElement(X509Extensions.KeyUsage);
            
            extensions.put(X509Extensions.AuthorityKeyIdentifier, new X509Extension(true, new DEROctetString(createAuthorityKeyId(info, new X509Name("CN=AU,O=Bouncy Castle,OU=Test 2"), 2))));
            extensions.put(X509Extensions.SubjectKeyIdentifier, new X509Extension(true, new DEROctetString(new SubjectKeyIdentifier(info))));
            extensions.put(X509Extensions.KeyUsage, new X509Extension(false, new DEROctetString(new KeyUsage(KeyUsage.dataEncipherment))));
            
            X509Extensions  ex = new X509Extensions(order, extensions);
            
            gen.setExtensions(ex);
            
            TBSCertificateStructure     tbs = gen.generateTBSCertificate();
            ByteArrayOutputStream       bOut = new ByteArrayOutputStream();
            ASN1OutputStream            aOut = new ASN1OutputStream(bOut);
            
            aOut.writeObject(tbs);
            
            if (!isSameAs(bOut.toByteArray(), v3Cert))
            {
                return new SimpleTestResult(false, getName() + ": failed v3 cert generation");
            }
            
            //
            // read back test
            //
            ASN1InputStream aIn = new ASN1InputStream(new ByteArrayInputStream(v2CertList));
            DERObject       o = aIn.readObject();
            
            bOut = new ByteArrayOutputStream();
            aOut = new ASN1OutputStream(bOut);
            
            aOut.writeObject(o);
            
            if (!isSameAs(bOut.toByteArray(), v2CertList))
            {
                return new SimpleTestResult(false, getName() + ": failed v3 cert read back test");
            }
            
            return new SimpleTestResult(true, getName() + ": Okay");
        }
        catch (Exception e)
        {
            return new SimpleTestResult(false, getName() + ": v3 cert list failed " + e.toString(), e);
        }
    }
    
    private TestResult tbsV2CertListGen()
    {
        try
        {
            V2TBSCertListGenerator  gen = new V2TBSCertListGenerator();
            
            gen.setIssuer(new X509Name("CN=AU,O=Bouncy Castle"));
            
            gen.addCRLEntry(new DERInteger(1), new Time(new Date(1000)), ReasonFlags.AA_COMPROMISE);
            
            gen.setNextUpdate(new Time(new Date(2000)));
            
            gen.setThisUpdate(new Time(new Date(500)));
            
            gen.setSignature(new AlgorithmIdentifier(PKCSObjectIdentifiers.sha1WithRSAEncryption, new DERNull()));
            
            //
            // extensions
            //
            Vector                  order = new Vector();
            Hashtable               extensions = new Hashtable();
            SubjectPublicKeyInfo    info = new SubjectPublicKeyInfo(new AlgorithmIdentifier(OIWObjectIdentifiers.elGamalAlgorithm, new ElGamalParameter(BigInteger.valueOf(1), BigInteger.valueOf(2))), new DERInteger(3));

            order.addElement(X509Extensions.AuthorityKeyIdentifier);
            order.addElement(X509Extensions.IssuerAlternativeName);
            order.addElement(X509Extensions.CRLNumber);
            order.addElement(X509Extensions.IssuingDistributionPoint);
            
            extensions.put(X509Extensions.AuthorityKeyIdentifier, new X509Extension(true, new DEROctetString(createAuthorityKeyId(info, new X509Name("CN=AU,O=Bouncy Castle,OU=Test 2"), 2))));
            extensions.put(X509Extensions.IssuerAlternativeName, new X509Extension(false, new DEROctetString(new GeneralNames(new DERSequence(new GeneralName(new X509Name("CN=AU,O=Bouncy Castle,OU=Test 3")))))));
            extensions.put(X509Extensions.CRLNumber, new X509Extension(false, new DEROctetString(new DERInteger(1))));
            extensions.put(X509Extensions.IssuingDistributionPoint, new X509Extension(true, new DEROctetString(new IssuingDistributionPoint(new DERSequence()))));

            X509Extensions          ex = new X509Extensions(order, extensions);
            
            gen.setExtensions(ex);
            
            TBSCertList                 tbs = gen.generateTBSCertList();
            ByteArrayOutputStream       bOut = new ByteArrayOutputStream();
            ASN1OutputStream            aOut = new ASN1OutputStream(bOut);
            
            aOut.writeObject(tbs);
            
            if (!isSameAs(bOut.toByteArray(), v2CertList))
            {
                return new SimpleTestResult(false, getName() + ": failed v2 cert list generation");
            }
            
            //
            // read back test
            //
            ASN1InputStream aIn = new ASN1InputStream(new ByteArrayInputStream(v2CertList));
            DERObject       o = aIn.readObject();
            
            bOut = new ByteArrayOutputStream();
            aOut = new ASN1OutputStream(bOut);
            
            aOut.writeObject(o);
            
            if (!isSameAs(bOut.toByteArray(), v2CertList))
            {
                return new SimpleTestResult(false, getName() + ": failed v2 cert list read back test");
            }
            
            return new SimpleTestResult(true, getName() + ": Okay");
        }
        catch (Exception e)
        {
            return new SimpleTestResult(false, getName() + ": v2 cert list failed " + e.toString(), e);
        }
    }
    
    public TestResult perform()
    {
        TestResult  res = tbsV1CertGen();
        
        if (!res.isSuccessful())
        {
            return res;
        }
        
        res = tbsV3CertGen();
        if (!res.isSuccessful())
        {
            return res;
        }
        
        return tbsV2CertListGen();
    }

    public String getName()
    {
        return "Generation";
    }
    
    public static void main(
        String[] args)
    {
        GenerationTest    test = new GenerationTest();
        TestResult      result = test.perform();

        System.out.println(result);
    }
}
