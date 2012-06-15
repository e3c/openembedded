DESCRIPTION = "A (de)compression library for the ZIP format"
SECTION = "console/utils"
LICENSE = "Info-ZIP"
PR = "r0"
SRC_URI = "${SOURCEFORGE_MIRROR}/project/infozip/UnZip%206.x%20%28latest%29/UnZip%206.0/unzip60.tar.gz"
S = "${WORKDIR}/unzip60"

BBCLASSEXTEND = "native"

NATIVE_INSTALL_WORKS = "1"

EXTRA_OEMAKE += "'LD=${CCLD} ${LDFLAGS}'"

do_compile() {
        oe_runmake -f unix/Makefile generic
}

do_install() {
        oe_runmake -f unix/Makefile install prefix=${D}${prefix}
	install -d ${D}${mandir}
	mv ${D}${prefix}/man/* ${D}${mandir}
}

do_install_append_pn-unzip() {
	mv ${D}${bindir}/unzip ${D}${bindir}/unzip.${PN}
}

pkg_postinst_${PN} () {
	update-alternatives --install ${bindir}/unzip unzip unzip.${PN} 100
}

pkg_prerm_${PN} () {
	update-alternatives --remove unzip unzip.${PN}
}

SRC_URI[md5sum] = "62b490407489521db863b523a7f86375"
SRC_URI[sha256sum] = "036d96991646d0449ed0aa952e4fbe21b476ce994abc276e49d30e686708bd37"
