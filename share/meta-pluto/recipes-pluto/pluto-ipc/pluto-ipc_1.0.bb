SUMMARY = "Pluto IPC demo Application"
DESCRIPTION = "Pluto IPC demo Application."


LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""

SRC_URI += " \
    file://pluto-ipc.c \
"

S = "${WORKDIR}"

do_compile() {
    ${CC} pluto-ipc.c -o pluto-ipc
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 pluto-ipc ${D}${bindir}
}

TARGET_CC_ARCH += "${LDFLAGS}"
FILES_${PN} += "${bindir}"
