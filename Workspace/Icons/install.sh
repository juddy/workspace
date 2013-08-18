INST_DIR="~/GNUstep/Library/Workspace/IconSets"
ICONS="xetos-icons"
ICONSET="xetos.iconset"
# quick and dirty script to install the icons & iconset for Workspace
echo "Copying Iconset file to Default location; ~/GNUstep/Library/Workspace/IconSets"
	mkdir $INST_DIR &&  echo "Copying Icons to default location (~/GNUstep/Library/Icons/itomatOS)"
	echo "For now, you'll have to add that path to your Workspace Defaults"
	echo "or with workspaceconf or workspace preferences."
		cp -aR $ICONS $INST_DIR && chown -R $(whoami):root $INST_DIR/$ICONS && chmod -R 755 $INST_DIR/$ICONS && echo "That should do it"
