import { Avatar, Box, Card, CardBody, CardHeader, Flex, Heading, Icon, Text } from "@chakra-ui/react"
import { ArrowUpIcon, ArrowDownIcon } from '@chakra-ui/icons'

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
    cardNumber: number
}
export function Player({ name, cardNumber }: PlayerProps) {
    return (
        <Box display='flex' flexDirection='row' justifyContent='center' alignItems='center' gap='10px'>
            <Avatar name={name} />

            <Card width='50px' height='75px'>
                <CardBody justifyContent='center' alignItems='center' justifySelf='center'>
                    <Text >{cardNumber}</Text>
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