FILESEXTRAPATHS_prepend := "${THISDIR}/configs:"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

PV = "${DISTRO_VERSION}${TDX_BUILDNB}"

# Always create vmdk and vdi images for the compatible targets
IMAGE_FSTYPES_append_intel-corei7-64 = " wic.vmdk wic.vdi"

inherit core-image distro_features_check extrausers

SRC_URI = "\
    file://sysctl-panic.conf \
    file://path-sbin.sh \
    file://sudoers \
    file://ostree-pending-reboot.service \
"

# let's make sure we have a good image..
REQUIRED_DISTRO_FEATURES = "pam systemd"

# Base packages
CORE_IMAGE_BASE_INSTALL += " \
    sudo \
"

fakeroot do_populate_rootfs_common_src () {
    # Allow sudo group users to use sudo
    install -m 0440 ${WORKDIR}/sudoers ${IMAGE_ROOTFS}${sysconfdir}/sudoers.d/lmp

    # Systemd service files
    install -m 0644 ${WORKDIR}/ostree-pending-reboot.service ${IMAGE_ROOTFS}${systemd_system_unitdir}/

    # Configs that are specific to this image
    install -m 0644 ${WORKDIR}/sysctl-panic.conf ${IMAGE_ROOTFS}${exec_prefix}/lib/sysctl.d/60-panic.conf

    # Useful for development
    install -d ${IMAGE_ROOTFS}${sysconfdir}/profile.d
    install -m 0644 ${WORKDIR}/path-sbin.sh ${IMAGE_ROOTFS}${sysconfdir}/profile.d/path-sbin.sh
}

IMAGE_PREPROCESS_COMMAND += "do_populate_rootfs_common_src; "

addtask rootfs after do_unpack

python () {
    # Ensure we run these usually noexec tasks
    d.delVarFlag("do_fetch", "noexec")
    d.delVarFlag("do_unpack", "noexec")
}

EXTRA_USERS_PARAMS = " \
useradd -P torizon torizon; \
usermod -a -G sudo,users,plugdev torizon; \
"
