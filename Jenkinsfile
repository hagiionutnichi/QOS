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
make setup
make bootimg
cd ..'''
      }
    }

  }
}