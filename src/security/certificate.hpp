/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/**
 * Copyright (C) 2013 Regents of the University of California.
 * @author: Yingdi Yu <yingdi@cs.ucla.edu>
 * @author: Jeff Thompson <jefft0@remap.ucla.edu>
 * See COPYING for copyright and distribution information.
 */

#ifndef NDN_SECURITY_CERTIFICATE_HPP
#define NDN_SECURITY_CERTIFICATE_HPP

#include "../common.hpp"
#include "../data.hpp"

#include "certificate-subject-description.hpp"
#include "certificate-extension.hpp"
#include "public-key.hpp"

namespace ndn {

class Certificate : public Data {
public:
  struct Error : public std::runtime_error { Error(const std::string &what) : std::runtime_error(what) {} };

  typedef std::vector<CertificateSubjectDescription> SubjectDescriptionList;
  typedef std::vector<CertificateExtension> ExtensionList;

  /**
   * The default constructor.
   */
  Certificate();

  /**
   * Create a Certificate from the content in the data packet.
   * @param data The data packet with the content to decode.
   */
  Certificate(const Data& data);

  /**
   * The virtual destructor.
   */
  virtual
  ~Certificate();

  inline void
  wireDecode(const Block &wire);

  /**
   * encode certificate info into content
   */
  void
  encode();

  /**
   * Add a subject description.
   * @param description The description to be added.
   */
  void
  addSubjectDescription(const CertificateSubjectDescription& description) { subjectDescriptionList_.push_back(description); }

  const SubjectDescriptionList&
  getSubjectDescriptionList() const { return subjectDescriptionList_; }

  SubjectDescriptionList&
  getSubjectDescriptionList() { return subjectDescriptionList_; }

  /**
   * Add a certificate extension.
   * @param extension the extension to be added
   */
  void
  addExtension(const CertificateExtension& extension) { extensionList_.push_back(extension); }

  const ExtensionList&
  getExtensionList() const { return extensionList_; }

  ExtensionList&
  getExtensionList() { return extensionList_; }

  void
  setNotBefore(const time::system_clock::TimePoint& notBefore) { notBefore_ = notBefore; }

  time::system_clock::TimePoint&
  getNotBefore() { return notBefore_; }

  const time::system_clock::TimePoint&
  getNotBefore() const { return notBefore_; }

  void
  setNotAfter(const time::system_clock::TimePoint& notAfter) { notAfter_ = notAfter; }

  time::system_clock::TimePoint&
  getNotAfter() { return notAfter_; }

  const time::system_clock::TimePoint&
  getNotAfter() const { return notAfter_; }

  void
  setPublicKeyInfo(const PublicKey& key) { key_ = key; }

  PublicKey&
  getPublicKeyInfo() { return key_; }

  const PublicKey&
  getPublicKeyInfo() const { return key_; }

  // virtual Name
  // getPublicKeyName() const = 0;

  /**
   * Check if the certificate is valid.
   * @return True if the current time is earlier than notBefore.
   */
  bool
  isTooEarly();

  /**
   * Check if the certificate is valid.
   * @return True if the current time is later than notAfter.
   */
  bool
  isTooLate();

  void
  printCertificate(std::ostream &os) const;

protected:
  void
  decode();

protected:
  SubjectDescriptionList subjectDescriptionList_;
  time::system_clock::TimePoint notBefore_;
  time::system_clock::TimePoint notAfter_;
  PublicKey key_;
  ExtensionList extensionList_;
};

inline void
Certificate::wireDecode(const Block &wire)
{
  Data::wireDecode(wire);
  decode();
}


inline std::ostream&
operator <<(std::ostream &os, const Certificate &cert)
{
  cert.printCertificate(os);
  return os;
}

} // namespace ndn

#endif //NDN_SECURITY_CERTIFICATE_HPP
