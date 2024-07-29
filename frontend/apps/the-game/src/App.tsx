import { useState } from "react"
import { Box, ChakraProvider, Tab, TabList, TabPanel, TabPanels, Tabs, theme } from "@chakra-ui/react"
import { getStatus } from "./utils/Common"
import { DebugPage } from "./components/DebugPage"
import { ColorModeSwitcher } from "./components/ColorModeSwitcher"
import { GamePage } from "./components/GamePage"

export function App() {
  const [state, setState] = useState<String[]>(['No state available'])

  const updateStatus = () => {
    getStatus().then(setState).catch(console.error)
  }

  return (
    <ChakraProvider theme={theme}>
      <Box display={'flex'}>
        <Tabs minWidth={'98%'} defaultIndex={1}>
          <TabList>
            <Tab>Debug Panel </Tab>
            <Tab>Game</Tab>
          </TabList>
          <TabPanels>
            <TabPanel>
              <DebugPage
                state={state}
                onUpdateStatusButtonClick={updateStatus} />
            </TabPanel>
            <TabPanel>
              <GamePage/>
            </TabPanel>
          </TabPanels>
        </Tabs>
        <ColorModeSwitcher justifySelf="flex-end" />
      </Box>
    </ChakraProvider>)
}
