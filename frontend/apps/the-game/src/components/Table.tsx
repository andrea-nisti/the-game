import { ArrowDownIcon, ArrowUpIcon } from "@chakra-ui/icons";
import { Box, Card, CardBody, Text } from "@chakra-ui/react";
import React from "react";

interface GameTableProp {
}

interface GameTableState {
    upCards: TableCard[]
    downCards: TableCard[]
}

export default class GameTable extends React.Component<GameTableProp, GameTableState> {
    constructor(props: GameTableProp) {
        super(props);

        this.state = {
            upCards: [
                new TableCard({ value: 1, direction: 'up' , index: 0}),
                new TableCard({ value: 1, direction: 'up' , index: 1})
            ],
            downCards: [
                new TableCard({ value: 99, direction: 'down', index: 2 }),
                new TableCard({ value: 99, direction: 'down', index: 3 })
            ]
        };
    }

    render() {
        return (
            <Box
                // sx={{ border: '10px solid white' }}
                justifyContent='center'
                alignItems='center'
                display='flex'
                flexDirection='column'
                gap='10px'>

                <Box display='flex' flexDirection='row' gap='10px'>
                    {this.state.upCards.map((card: TableCard) => (
                        <TableCard key={card.state.index} value={card.state.value} direction={card.state.direction} />
                    ))}
                </Box>
                <Box display='flex' flexDirection='row' gap='10px'>
                    {this.state.downCards.map((card: TableCard) => (
                        <TableCard key={card.state.index} value={card.state.value} direction={card.state.direction} />
                    ))}
                </Box>
            </Box>
        )
    }
}


interface TableCardProp {
    value: number;
    index?: number;
    direction: string;
}

interface TableCardState {
    value: number;
    index?: number
    direction: string;
}

class TableCard extends React.Component<TableCardProp, TableCardState> {
    constructor(props: TableCardProp) {
        super(props);

        this.state = {
            value: props.value,
            index: props.index,
            direction: props.direction
        };
    }

    render() {
        return (
            <Card variant='outline' align='center' width='100px' height='170px' >
                <CardBody fontSize='30px' alignItems='stretch' alignContent='center'>
                    {this.state.direction == 'up' ? <ArrowUpIcon /> : <ArrowDownIcon />}
                    <Text align='center'> {this.state.value} </Text>
                </CardBody>
            </Card>
        )
    }
}