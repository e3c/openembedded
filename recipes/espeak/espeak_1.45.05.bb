DESCRIPTION = "eSpeak is a compact open source software speech synthesizer"
SECTION = "base"
LICENSE = "GPLv2"
DEPENDS = "portaudio-v19"

S = "${WORKDIR}/espeak-${PV}-source"

PR = "r4"

inherit siteinfo

SRC_URI = "${SOURCEFORGE_MIRROR}/espeak/espeak-${PV}-source.zip \
    file://fix-mbrola-synth.patch \
    file://adjust-mbrola-samplerate.patch"

LEAD_SONAME = "libespeak.so"

CXXFLAGS += "-DUSE_PORTAUDIO"
TARGET_CC_ARCH += "${LDFLAGS}"

do_configure() {
       #  "speak" binary, a TTS engine, uses portaudio in either APIs V18 or V19, use V19
       cp "${S}/src/portaudio19.h" "${S}/src/portaudio.h"
       cp -prf "${S}/dictsource" "${S}/espeak-data/"
}

do_compile() {
       cd src
       oe_runmake
}

do_install() {
        install -d ${D}${bindir}
        install -d ${D}${libdir}
        install -d ${D}${includedir}
        install -d ${D}${datadir}/espeak-data

        # we do not ship "speak" binary though.
        install -m 0755 ${S}/src/espeak ${D}${bindir}
        install -m 0644 ${S}/src/speak_lib.h ${D}${includedir}
        oe_libinstall -so -C src libespeak ${D}${libdir}

        cp -prf ${S}/espeak-data/* ${D}${datadir}/espeak-data
}

do_stage() {
        install -d ${STAGING_INCDIR}/espeak
        install -m 0644 ${S}/src/speak_lib.h ${STAGING_INCDIR}/espeak/
        oe_libinstall -so -C src libespeak ${STAGING_LIBDIR}
}

FILES_${PN} += "${datadir}/espeak-data"

SRC_URI[md5sum] = "00692552d54f1849a1a0ad21ae3fc820"
SRC_URI[sha256sum] = "ce7e7ff85944f83343d11ff0cb6af7fcac1a63d3f3e2271f516544fa297b8f97"
