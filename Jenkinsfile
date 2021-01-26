pipeline {
  agent any
  stages {
    stage('UEFI') {
      steps {
        sh '''cd gnu-efi
make
make bootloader
cd ..'''
      }
    }

    stage('Kernel') {
      steps {
        sh '''cd kernel
make kernel
make buildimg
cd ..'''
      }
    }

    stage('Upload Image') {
      steps {
        archiveArtifacts 'kernel/bin/QOS.img'
      }
    }

    stage('Upload Kernel') {
      steps {
        archiveArtifacts 'kernel/bin/kernel.elf'
      }
    }

  }
}