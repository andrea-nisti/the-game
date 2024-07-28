import * as React from "react"
import { useState } from "react"
import {
  Box,
  Text,
  VStack,
  Grid,
  Button,
  Card,
  CardHeader,
  Heading,
  CardBody,
} from "@chakra-ui/react"


type DebugPageProps = {
    state: String[]
    onUpdateStatusButtonClick?: () => void
}

export function DebugPage(props : DebugPageProps) {
  return (
      <Box marginTop={'10px'} fontSize="xl">
          <VStack align='flex-start' spacing={8}>
            <Button onClick={props.onUpdateStatusButtonClick}> GET STATUS </Button>
            <Card width='1000px' key={'size'} size='md'>
              <CardHeader>
                <Heading size='md' sx={{display:'flex'}}> Current Status </Heading>
              </CardHeader>
              <CardBody>
                {props.state.map((item) => <Text> {item} </Text>)}
              </CardBody>
            </Card>
          </VStack>
      </Box>
  )
}
