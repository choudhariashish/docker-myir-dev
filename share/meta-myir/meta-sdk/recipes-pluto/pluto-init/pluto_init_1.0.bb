SUMMARY = "Init script for pluto apps"
DESCRIPTION = "Init script for pluto apps."


LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""

SRC_URI += " \
    file://helloworld.c \
    file://text.txt \
    file://pluto-init-script \
    file://pluto-init.service \
"

inherit systemd
RDEPENDS_${PN} += " bash "
SYSTEMD_AUTO_ENABLE = "enable"
SYSTEMD_SERVICE_${PN} = "pluto-init.service"
FILES_${PN} += "${systemd_unitdir}/system/pluto-init.service"


S = "${WORKDIR}"

do_compile() {
    ${CC} helloworld.c -o helloworld
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 helloworld ${D}${bindir}
    install -m 0755 pluto-init-script ${D}${bindir}

    install -d ${D}/${systemd_unitdir}/system
    install -m 0644 ${WORKDIR}/pluto-init.service ${D}/${systemd_unitdir}/system
}

TARGET_CC_ARCH += "${LDFLAGS}"
FILES_${PN} += "${bindir}"
