##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=test
ConfigurationName      :=Debug
WorkspacePath          := "/home/dinu/Documents/test"
ProjectPath            := "/home/dinu/Documents/test"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=dinu
Date                   :=05/29/14
CodeLitePath           :="/home/dinu/.codelite"
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="test.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main$(ObjectSuffix) $(IntermediateDirectory)/sdd$(ObjectSuffix) $(IntermediateDirectory)/tabela_mainTabela$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dinu/Documents/test/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main$(ObjectSuffix) -MF$(IntermediateDirectory)/main$(DependSuffix) -MM "main.cpp"

$(IntermediateDirectory)/main$(PreprocessSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main$(PreprocessSuffix) "main.cpp"

$(IntermediateDirectory)/sdd$(ObjectSuffix): sdd.cpp $(IntermediateDirectory)/sdd$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dinu/Documents/test/sdd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sdd$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sdd$(DependSuffix): sdd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sdd$(ObjectSuffix) -MF$(IntermediateDirectory)/sdd$(DependSuffix) -MM "sdd.cpp"

$(IntermediateDirectory)/sdd$(PreprocessSuffix): sdd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sdd$(PreprocessSuffix) "sdd.cpp"

$(IntermediateDirectory)/tabela_mainTabela$(ObjectSuffix): ../tabela/mainTabela.cpp $(IntermediateDirectory)/tabela_mainTabela$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dinu/Documents/tabela/mainTabela.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tabela_mainTabela$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tabela_mainTabela$(DependSuffix): ../tabela/mainTabela.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/tabela_mainTabela$(ObjectSuffix) -MF$(IntermediateDirectory)/tabela_mainTabela$(DependSuffix) -MM "../tabela/mainTabela.cpp"

$(IntermediateDirectory)/tabela_mainTabela$(PreprocessSuffix): ../tabela/mainTabela.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tabela_mainTabela$(PreprocessSuffix) "../tabela/mainTabela.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/main$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/sdd$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/sdd$(DependSuffix)
	$(RM) $(IntermediateDirectory)/sdd$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/tabela_mainTabela$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/tabela_mainTabela$(DependSuffix)
	$(RM) $(IntermediateDirectory)/tabela_mainTabela$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) ".build-debug/test"


