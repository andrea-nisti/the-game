import { Avatar, Box, Card, CardBody, CardHeader, Flex, Heading, Icon, Text } from "@chakra-ui/react"
import { ArrowUpIcon, ArrowDownIcon } from '@chakra-ui/icons'
import "./GamePage.css"
import { useState } from "react"
import { generateRandomNumbers } from "../utils/Common"


type GamePageProps = {

}

export function GamePage(props: GamePageProps) {
    const [userCardsValues, setUserCardsValue] = useState<number[]>(generateRandomNumbers(6, 1, 99))

    return (

        <Box
            sx={{ border: '10px solid red' }}
            display='flex'
            flexDirection='column-reverse'
            // className="mainGamePage"
            height='94vh'
            width='98.5vw'
            gap='100px'>

            <Box
                // sx={{ border: '10px solid white' }}
                justifyContent='center'
                display='flex'
                flexDirection='row'
                gap='10px'
                marginBottom='100px'>

                {userCardsValues.map((number) => (
                    <UserCard cardValue={number} />

                ))}
            </Box>

            <Box
                // sx={{ border: '10px solid white' }}
                justifyContent='center'
                alignItems='center'
                display='flex'
                flexDirection='column'
                gap='10px'>

                <Box display='flex' flexDirection='row' gap='10px'>
                    <TableCard value={1} direction="up" />
                    <TableCard value={1} direction="up" />
                </Box>
                <Box display='flex' flexDirection='row' gap='10px'>
                    <TableCard value={99} direction="down" />
                    <TableCard value={99} direction="down" />
                </Box>
            </Box>
            <Box
                // sx={{ border: '10px solid white' }}
                justifyContent='center'
                alignItems='flex-end'
                display='flex'
                flexDirection='row'
                gap='50px'>
                <Player name="Prosepio Finacchioni" cards={3} />
                <Player name="Marchionne Pampalone" cards={3} />
                <Player name="Frano Sticchiano" cards={3} />
            </Box>
        </Box>

    )
}


type SingleCardProps = {
    cardValue: number
}

export function UserCard(props: SingleCardProps) {
    return (
        <Card variant='filled' align='center' width='100px' height='170px' >

            <CardBody fontSize='30px' alignItems='stretch' alignContent='center'>
                <Text align='center'> {props.cardValue} </Text>
            </CardBody>
        </Card>
    )
}


type PlayerProps = {
    name: string
    cards: number
}
export function Player({ name, cards }: PlayerProps) {
    return (
        <Box display='flex' flexDirection='row' justifyContent='center' alignItems='center' gap='10px'>
            <Avatar name={name} />

            <Card width='50px' height='75px'>
                <CardBody justifyContent='center' alignItems='center' justifySelf='center'>
                    <Text>{cards}</Text>
                </CardBody>
            </Card>
        </Box>
    )
}

type TableCardProps = {
    value: number,
    direction: string
}

export function TableCard({ value, direction }: TableCardProps) {

    return (
        <Card variant='outline' align='center' width='100px' height='170px' >

            <CardBody fontSize='30px' alignItems='stretch' alignContent='center'>
                {direction == 'up' ? <ArrowUpIcon /> : <ArrowDownIcon />}
                <Text align='center'> {value} </Text>
            </CardBody>
        </Card>
    )
}