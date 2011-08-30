DESCRIPTION = "Support for the Linux 2.6.x ALSA Sound System"
SECTION = "devel/python"
DEPENDS = "alsa-lib"
PRIORITY = "optional"
LICENSE = "GPL"
SRCNAME = "pyalsaaudio"
PR = "ml0"

SRC_URI = "${SOURCEFORGE_MIRROR}/pyalsaaudio/${SRCNAME}-${PV}.tar.gz"
S = "${WORKDIR}/${SRCNAME}-${PV}"

inherit distutils

SRC_URI[md5sum] = "94811fc4de883996eac4046d36fe2364"
SRC_URI[sha256sum] = "f048dd8ed7e2e400a90ba945b8dbff170448060617a7d5fd1d8ef292cf686c4d"
