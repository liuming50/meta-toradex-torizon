LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI_append = " \
	file://move-non-trivial-secondaryconfig-ctor-out-of-header.patch \
	file://initial-support-for-docker-compose-secondaries.patch \
"
