//
//  LottoData.swift
//  FSCalendar_practice
//
//  Created by 송선진 on 2022/10/19.
//

import UIKit

enum LottoType: String {
    case lotto = "로또"
    case instantLotto = "스피또"
}

struct LottoData {
    var buyDate: String
    var lottoType: LottoType
    var lottoAmount: Int
    
    static var rowData: [LottoData] = [
        
        LottoData(buyDate: "2022-10-19", lottoType: .lotto, lottoAmount: 5000),
        LottoData(buyDate: "2022-10-04", lottoType: .lotto, lottoAmount: 5000),
        LottoData(buyDate: "2022-10-25", lottoType: .lotto, lottoAmount: 5000),
        LottoData(buyDate: "2022-10-11", lottoType: .lotto, lottoAmount: 5000),
        LottoData(buyDate: "2022-10-30", lottoType: .lotto, lottoAmount: 5000)
    ]
}

