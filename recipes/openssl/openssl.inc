DESCRIPTION = "Secure Socket Layer (SSL) binary and related cryptographic tools."
HOMEPAGE = "http://www.openssl.org/"
LICENSE = "openssl"
SECTION = "libs/network"

SRC_URI = "http://www.openssl.org/source/openssl-${PV}.tar.gz;name=src"
S = "${WORKDIR}/openssl-${PV}"

inherit siteinfo

INC_PR = "r14"

AR_append = " r"
CFLAG = "${@base_conditional('SITEINFO_ENDIANNESS', 'le', '-DL_ENDIAN', '-DB_ENDIAN', d)} \
	-DTERMIO ${FULL_OPTIMIZATION} -Wall"

# -02 does not work on mipsel: ssh hangs when it tries to read /dev/urandom
CFLAG_mtx-1 := "${@'${CFLAG}'.replace('-O2', '')}"
CFLAG_mtx-2 := "${@'${CFLAG}'.replace('-O2', '')}"

export DIRS = "crypto ssl apps"
export EX_LIBS = "-lgcc -ldl"
export AS = "${CC} -c"

PACKAGES =+ "libcrypto libssl ${PN}-misc openssl-conf"
FILES_libcrypto = "${libdir}/libcrypto.so.*"
FILES_libssl = "${libdir}/libssl.so.*"
FILES_${PN}-misc = "${libdir}/ssl/misc"

# Add the openssl.cnf file to the openssl-conf package.  Make the libcrypto
# package RRECOMMENDS on this package.  This will enable the configuration
# file to be installed for both the base openssl package and the libcrypto
# package since the base openssl package depends on the libcrypto package.
FILES_openssl-conf = "${libdir}/ssl/openssl.cnf"
CONFFILES_openssl-conf = "${libdir}/ssl/openssl.cnf"
RRECOMMENDS_libcrypto += "openssl-conf"

do_configure_prepend_darwin () {
	sed -i -e '/version-script=openssl\.ld/d' Configure
}

do_configure () {
	cd util
	perl perlpath.pl ${bindir}
	cd ..
	ln -sf apps/openssl.pod crypto/crypto.pod ssl/ssl.pod doc/

	os=${HOST_OS}
	if [ `echo $os | grep -q linux; echo $?` -eq 0 ]; then
		os=linux
	fi
	target="$os-${HOST_ARCH}"
	case $target in
	linux-arm)
		target=linux-elf-arm
		;;
	linux-armeb)
		target=linux-elf-armeb
		;;
	linux-sh3)
		target=debian-sh3
		;;
	linux-sh4)
		target=debian-sh4
		;;
	linux-i486)
		target=debian-i386-i486
		;;
	linux-i586 | linux-viac3)
		target=debian-i386-i586
		;;
	linux-i686)
		target=debian-i386-i686/cmov
		;;
	linux-mips)
		target=debian-mips
		;;
	linux-mipsel)
		target=debian-mipsel
		;;
	linux-powerpc | linux-powerpc64)
		target=linux-ppc
		;;
	linux-supersparc)
		target=linux-sparcv8
		;;
	linux-sparc)
		target=linux-sparcv8
		;;
	darwin-i386)
		target=darwin-i386-cc
		;;
	esac
	# inject machine-specific flags
	sed -i -e "s|^\(\"$target\",\s*\"[^:]\+\):\([^:]\+\)|\1:${CFLAG}|g" Configure
        useprefix=${prefix}
        if [ "x$useprefix" = "x" ]; then
                useprefix=/
        fi        
	perl ./Configure ${EXTRA_OECONF} shared --prefix=$useprefix --openssldir=${libdir}/ssl $target

	eval "${@base_contains('DISTRO_FEATURES', 'largefile', '', 'sed -i -e "/_FILE_OFFSET_BITS/,/#endif/d" ${S}/crypto/bio/bss_file.c', d)}"
	eval "${@base_contains('DISTRO_FEATURES', 'ipv6', '', 'sed -i -e "/AF_INET6/,/break/d" ${S}/crypto/bio/bss_dgram.c', d)}"
}

do_compile () {
	oe_runmake
}

do_install () {
	oe_runmake INSTALL_PREFIX="${D}" install

	# On x86_64, move lib/* to lib64
	if [ "${libdir}" != "${prefix}/lib" ]
	then
		install -d ${D}${libdir} ${D}${libdir}/pkgconfig
		mv ${D}${prefix}/lib/lib* ${D}${libdir}
		mv ${D}${prefix}/lib/pkgconfig/*.pc ${D}${libdir}/pkgconfig
	fi

	oe_libinstall -so libcrypto ${D}${libdir}
	oe_libinstall -so libssl ${D}${libdir}

	install -d ${D}${includedir}
	cp -L -R include/openssl ${D}${includedir}
}
