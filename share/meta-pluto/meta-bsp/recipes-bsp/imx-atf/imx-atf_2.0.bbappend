SRCBRANCH = "imx_5.4.3_2.0.0"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

IMX8MM_UART4_PATCH="file://0001-add-uart4-A53-support.patch"

ATF_SRC ?= "git://github.com/nxp-imx/imx-atf.git;protocol=https"
SRC_URI = "${ATF_SRC};branch=${SRCBRANCH} \
           file://0001-Allow-BUILD_STRING-to-be-set-in-.revision-file.patch \
           ${@bb.utils.contains('MACHINENAME', 'myd-imx8mm', '${IMX8MM_UART4_PATCH}', '', d)} \
           ${@bb.utils.contains('MACHINENAME', 'myd-jx8mm', '${IMX8MM_UART4_PATCH}', '', d)} \
           ${@bb.utils.contains('MACHINENAME', 'myd-jx8mx', 'file://0001-FEAT-myd_jx8mx-imx8mq-2GDDR-configure.patch', '', d)} \
"
SRCREV = "f1a195b5cce64365a7227557a9009a4f545aa02d"

PLATFORM_mx8x   = "imx8qx"
# Override PLATFORM_mx8qxp from the included recipe
PLATFORM_mx8qxp = "imx8qx"
PLATFORM_mx8mn  = "imx8mn"
PLATFORM_mx8mp  = "imx8mp"
PLATFORM_imx8dxlevk = "imx8dxl"
PLATFORM_mx8dxlevk-phantom = "imx8qx"
PLATFORM_mx8dx = "imx8dx"

BUILD_OPTEE = "${@bb.utils.contains('MACHINE_FEATURES', 'optee', 'true', 'false', d)}"

do_compile_append() {
    if [ "${BUILD_OPTEE}" = "true" ]; then
        oe_runmake clean BUILD_BASE=build-optee
        oe_runmake BUILD_BASE=build-optee SPD=opteed bl31
    fi
}

do_deploy_append () {
    if [ "${BUILD_OPTEE}" = "true" ]; then
        install -m 0644 ${S}/build-optee/${PLATFORM}/release/bl31.bin ${DEPLOYDIR}/${BOOT_TOOLS}/bl31-${PLATFORM}.bin-optee
    fi
}
