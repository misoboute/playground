#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

namespace csb
{

template <class Dur = std::chrono::milliseconds>
class Timer
{
public:
    using Duration = Dur;
    using Rep = typename Duration::rep;

    Timer(const char* name = nullptr, bool start = true)
    {
        if (name != nullptr)
        {
            m_Name = name;
        }
        if (start)
        {
            Start();
        }
    }

    void Start()
    {
        m_StartTime = ClockT::now();
    }

    Rep Check(Rep maxTime = Rep())
    {
        auto elapsed = std::chrono::duration_cast<Duration>(
            ClockT::now() - m_StartTime).count();
        std::cerr << "Timer ";
        if (!m_Name.empty())
        {
            std::cerr << "(" << m_Name << ") ";
        }
        std::cerr << "measured: " << elapsed << "ms" << std::endl;
        if (elapsed > maxTime && maxTime > Rep())
        {
            std::ostringstream msg;
            msg << "Timeout on timer";
            if (!m_Name.empty())
            {
                msg << " (" << m_Name << ")";
            }
            msg << " - elaped: " << elapsed << ", maxTime: " << maxTime;
            throw std::runtime_error(msg.str().c_str());
        }
        return elapsed;
    }

private:
    using ClockT = std::chrono::steady_clock;
    
    ClockT::time_point m_StartTime;
    std::string m_Name;
};

class MathUtils
{
public:
    static constexpr double Epsilon = 0.000001;
    static double Radians(double degrees) { return degrees * Pi / 180; }
    static double Degrees(double degrees) { return degrees / Pi * 180; }

    static bool IsZeroWithEpsilon(double val, double eps = Epsilon) 
    { 
        return std::abs(val) < eps; 
    }

    static bool AreEqualWithEpsilon(double a, double b, double eps = Epsilon) 
    { 
        return std::abs(a - b) < eps; 
    }

    template<typename T>
    static std::vector<T> SolveQuadraticEquation(T a, T b, T c)
    {
        std::vector<T> roots;

        auto delta = b * b - 4 * a * c;
        if (delta < 0)
        {
            return roots;
        }

        roots.push_back((-b + std::sqrt(delta)) / (2 * a));
        if (delta > 0)
        {
            roots.push_back((-b - std::sqrt(delta)) / (2 * a));
        }

        return roots;
    }

    template<typename T>
    static std::vector<T> FindEquationRoots(
        std::function<T(T)> eqFunc, T lowerBound, T upperBound, int maxRoots)
    {
        std::vector<T> roots;
        return roots;
    }

    static constexpr double Pi = 3.14159265;
};

template <class T, size_t Dim>
class Vec
{
public:
    using ElemT = T;
    static constexpr size_t Dimensions = Dim;
    using VecT = Vec<ElemT, Dimensions>;

    Vec()
    {
    }

    Vec(ElemT init)
    {
        std::fill(std::begin(m_Elems), std::end(m_Elems), init);
    }

    template<class... U>
    explicit Vec(U... in)
    : m_Elems{ElemT(in)...}
    {
    }

    Vec(const VecT& other)
    {
        std::copy(std::cbegin(other.m_Elems)
            , std::cend(other.m_Elems), std::begin(m_Elems));
    }

    VecT& operator=(const VecT& other)
    {
        std::copy(std::cbegin(other.m_Elems)
            , std::cend(other.m_Elems), std::begin(m_Elems));
        return *this;
    }

    const ElemT& operator[](size_t i) const { return m_Elems[i]; }
    ElemT& operator[](size_t i) { return m_Elems[i]; }

    VecT operator+(const VecT& other) const
    {
        auto result = *this;
        result += other;
        return result;
    }

    VecT& operator+=(const VecT& other)
    {
        for (size_t i = 0; i < Dimensions; ++i)
            m_Elems[i] += other.m_Elems[i];
        return *this;
    }

    VecT operator-(const VecT& other) const
    {
        auto result = *this;
        result -= other;
        return result;
    }

    VecT& operator-=(const VecT& other)
    {
        for (size_t i = 0; i < Dimensions; ++i)
            m_Elems[i] -= other.m_Elems[i];
        return *this;
    }

    VecT operator-() const
    {
        VecT result;
        for (size_t i = 0; i < Dimensions; ++i)
            result[i] = -m_Elems[i];
        return result;
    }

    double operator*(const VecT& other) const
    {
        return Dot(other);
    }

    VecT& operator/=(ElemT den)
    {
        for (size_t i = 0; i < Dimensions; ++i)
            m_Elems[i] /= den;
        return *this;
    }

    VecT operator/(ElemT den) const
    {
        auto result = *this;
        result /= den;
        return result;
    }

    bool operator==(const VecT other) const
    {
        for (size_t i = 0; i < Dimensions; ++i)
        {
            if (!MathUtils::AreEqualWithEpsilon(m_Elems[i], other.m_Elems[i]))
            {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const VecT other) const
    {
        return !(*this == other);
    }

    ElemT Dot(const VecT& other) const
    {
        return std::inner_product(
            std::cbegin(m_Elems), std::cend(m_Elems), 
            std::cbegin(other.m_Elems), ElemT());
    }

    VecT Cross(const VecT& other) const
    {
        static_assert(
            Dimensions == 3, "Cross product is defined only for 3D vectors.");
        return VecT{ 
            m_Elems[1] * other[2] - other[1] * m_Elems[2],
            m_Elems[2] * other[0] - other[2] * m_Elems[0],
            m_Elems[0] * other[1] - other[0] * m_Elems[1],
        };
    }

    bool IsNull() const
    {
        for (size_t i = 0; i < Dimensions; ++i)
        {
            if (!MathUtils::IsZeroWithEpsilon(m_Elems[i]))
            {
                return false;
            }
        }
        return true;
    }

    ElemT GetNorm() const
    {
        return std::sqrt(Dot(*this));
    }

    void Normalize()
    {
        const auto norm = GetNorm();
        if (norm > 0)
        {
            for (auto& x : m_Elems)
                x /= norm;
        }
    }

    VecT GetNormalized() const
    {
        VecT result(*this);
        result.Normalize();
        return result;
    }

    VecT GetNormalizedOrZero() const
    {
        const auto norm = GetNorm();
        if (norm > 0)
        {
            VecT result;
            for (auto& x : result.m_Elems)
                x /= norm;
            return result;
        }
        return VecT(ElemT{});
    }

    VecT Scale(ElemT scale) const
    {
        VecT result(*this);
        for (size_t i = 0; i < Dimensions; ++i)
            result[i] = m_Elems[i] * scale;
        return result;
    }

    template <class OtherT, size_t OtherDim>
    friend class Vec;

    template<size_t NewDims>
    constexpr Vec<ElemT, NewDims> ChangeDims()
    {
        Vec<ElemT, NewDims> result;
        if (NewDims > Dimensions)
        {
            auto firstZero = std::copy_n(std::cbegin(m_Elems), Dimensions
                , std::begin(result.m_Elems));
            std::fill(firstZero, std::end(result.m_Elems), ElemT());
        }
        return result;
    }

    double AngleTo(const VecT& other)
    {
        return std::acos(GetNormalized() * other.GetNormalized());
    }

private:
    ElemT m_Elems[Dimensions];
};

template<class T, size_t N, class ScalarT>
Vec<T, N> operator*(const Vec<T, N>& v, ScalarT s)
{
    return v.Scale(s);
}

template<class T, size_t N, class ScalarT>
Vec<T, N> operator*(ScalarT s, const Vec<T, N>& v)
{
    return v.Scale(s);
}

template<class T, size_t N>
std::istream& operator>>(std::istream& is, Vec<T, N>& v)
{
    for (size_t i = 0; i < N; ++i)
        is >> v[i];
    return is;
}

template<class T, size_t N>
std::ostream& operator<<(std::ostream& os, const Vec<T, N>& v)
{
    os << "Vec(";
    for (size_t i = 0; i < N - 1; ++i)
        os << v[i] << ", ";
    return os << v[N - 1] << ")";
}

template <class T, size_t R, size_t C>
class Matrix
{
public:
    using ElemT = T;
    static constexpr size_t Rows = R;
    static constexpr size_t Cols = C;
    using MatrixT = Matrix<ElemT, Rows, Cols>;
    using ElemsArrayT = ElemT[Rows][Cols];

    static constexpr bool IsSquare()
    {
        return Cols == Rows;
    }

    Matrix()
    : m_ArrayRows(Rows)
    , m_ArrayCols(Cols)
    , m_Elems(AllocElemsArray())
    {
        InitMaps();
    }

    explicit Matrix(ElemT val)
    : m_ArrayRows(Rows)
    , m_ArrayCols(Cols)
    , m_Elems(AllocElemsArray())
    {
        InitMaps();
        std::fill_n(m_Elems.get(), Rows * Cols, val);
    }

    explicit Matrix(ElemT valArr[Rows][Cols])
    : m_ArrayRows(Rows)
    , m_ArrayCols(Cols)
    , m_Elems(AllocElemsArray())
    {
        InitMaps();
        std::copy_n(&valArr[0][0], Rows * Cols, m_Elems.get());
    }

    const ElemT& operator()(size_t row, size_t col) const
    {
        return At(row, col);
    }

    ElemT& operator()(size_t row, size_t col)
    {
        return At(row, col);
    }

    const ElemT& At(size_t row, size_t col) const
    {
        return *(m_Elems.get() + m_RowsMap[row] * m_ArrayCols + m_ColsMap[col]);
    }

    ElemT& At(size_t row, size_t col)
    {
        return *(m_Elems.get() + m_RowsMap[row] * m_ArrayCols + m_ColsMap[col]);
    }
    
    class Multiply
    {
    public:
        template<class U, class V>
        auto operator()(const U& u, const V& v) { return u * v; }
    };
    
    template<class ElemT2, size_t Cols2>
    Matrix<std::invoke_result_t<Multiply, ElemT, ElemT2>, Rows, Cols2>
    operator*(const Matrix<ElemT2, Cols, Cols2>& other)
    {
        using ProdElemT = std::invoke_result_t<Multiply, ElemT, ElemT2>;
        Matrix<std::invoke_result_t<Multiply, ElemT, ElemT2>, Rows, Cols2> prod;
        for (size_t i = 0; i < Rows; ++i)
        {
            for (size_t j = 0; j < Cols2; ++j)
            {
                prod(i, j) = ProdElemT();
                for (size_t k = 0; k < Cols; ++k)
                {
                    prod(i, j) += At(i, k) * other(k, j);
                }
            }
        }
        return prod;
    }

    template <class InvMatT>
    bool CalcInverse(InvMatT& inv) const
    {
        static_assert(IsSquare(),
            "Can't compute inverse of a non-square matrix");

        auto det = CalcDeterminant();
        if (det == 0)
        {
            return false;
        }

        for (size_t i = 0; i < Rows; ++i)
        {
            for (size_t j = 0; j < Cols; ++j) 
            {
                const auto sign = (i + j) % 2 == 0 ? 1 : -1;
                inv(j, i) += sign * static_cast<typename InvMatT::ElemT>(
                    MaskRowCol(i, j).CalcDeterminant()) / det;
            }
        }
        return true;
    }

    ElemT CalcDeterminant() const
    {
        static_assert(IsSquare(),
            "Can't compute determinant of a non-square matrix");

        if (Rows == 1)
            return At(0, 0);
        
        auto det = ElemT();
        for (size_t i = 0; i < Rows; ++i)
        {
            const auto sign = (i % 2 == 0) ? 1 : -1;
            det += sign * At(i, 0) * MaskRowCol(i, 0).CalcDeterminant();
        }
        return det;
    }

    Matrix<ElemT, (Rows > 1 ? Rows - 1 : Rows), (Cols > 1 ? Cols - 1 : Cols)> 
    MaskRowCol(size_t row, size_t col) const
    {
        assert(Rows > 1 && Cols > 1);

        auto rowsMap = ElemMapT(m_RowsMap.cbegin(), m_RowsMap.cbegin() + row);
        rowsMap.insert(
            rowsMap.cend(), m_RowsMap.cbegin() + row + 1, m_RowsMap.cend());

        auto colsMap = ElemMapT(m_ColsMap.cbegin(), m_ColsMap.cbegin() + col);
        colsMap.insert(
            colsMap.cend(), m_ColsMap.cbegin() + col + 1, m_ColsMap.cend());

        return { m_Elems, m_ArrayRows, m_ArrayCols, 
            std::move(rowsMap), std::move(colsMap) };
    }

    friend class Matrix<ElemT, Rows + 1, Cols + 1>;

private:
    using ElemsPtrT = std::shared_ptr<ElemT>;
    using ElemMapT = std::vector<size_t>;

    static ElemsPtrT AllocElemsArray()
    {
        return {new ElemT[Rows * Cols], [](auto ptr) { delete[] ptr; } };
    }

    Matrix(const ElemsPtrT& elems, 
        const size_t arrayRows, const size_t arrayCols,
        ElemMapT rowsMap, ElemMapT colsMap)
    : m_ArrayRows(arrayRows)
    , m_ArrayCols(arrayCols)
    , m_Elems(elems)
    , m_RowsMap(std::move(rowsMap))
    , m_ColsMap(std::move(colsMap))
    {
        assert(std::is_sorted(m_RowsMap.cbegin(), m_RowsMap.cend()));
        assert(std::is_sorted(m_ColsMap.cbegin(), m_ColsMap.cend()));
    }

    void InitMaps()
    {
        m_RowsMap.resize(Rows);
        std::iota(m_RowsMap.begin(), m_RowsMap.end(), 0);
        m_ColsMap.resize(Cols);
        std::iota(m_ColsMap.begin(), m_ColsMap.end(), 0);
    }

    const size_t m_ArrayRows;
    const size_t m_ArrayCols;
    ElemsPtrT m_Elems;
    ElemMapT m_RowsMap;
    ElemMapT m_ColsMap;
};

template<class T, size_t R, size_t C>
std::ostream& operator<<(std::ostream& os, const Matrix<T, R, C>& m)
{
    os << "Matrix<" << R << ", " << C << "> (";
    for (size_t i = 0; i < R; ++i)
    {
        os << " (";
        for (size_t j = 0; j < C - 1; ++j)
        {
            os << m(i, j) << ", ";
        }
        os << m(i, C - 1) << (i < R - 1 ? "), " : ") )");
    }
    return os;
}

class Pod
{
public:
    using VecT = Vec<double, 2>;

    const VecT& GetPos() const { return m_Pos; }
    void SetPos(const VecT& newPos) { m_Pos = newPos; }

    const VecT& GetVel() const { return m_Vel; }
    void SetVel(const VecT& newVel) { m_Vel = newVel; }

    double GetAngle() const { return m_Angle; }
    void SetAngle(double newAngle) { m_Angle = newAngle; }

    int GetCheckPointId() const { return m_CheckPointId; }
    void SetCheckPointId(int newCheckPointId) { 
        m_CheckPointId = newCheckPointId; }

private:
    friend std::istream& operator>>(std::istream& is, Pod& p);

    VecT m_Pos;
    VecT m_Vel;
    double m_Angle = 0;
    int m_CheckPointId = 0;
};

std::istream& operator>>(std::istream& is, Pod& p)
{
    return is >> p.m_Pos >> p.m_Vel >> p.m_Angle >> p.m_CheckPointId;
}

class Pilot;

class GameLoop
{
public:
    static constexpr size_t NumberOfPods = 2;

    using VecT = Pod::VecT;
    using CheckPointColT = std::vector<VecT>;
    using PodColT = Pod[NumberOfPods];
    using PilotPtrT = std::shared_ptr<Pilot>;
    using PilotColT = PilotPtrT[NumberOfPods];

    static GameLoop FromStdInput();

    GameLoop(int laps, std::vector<VecT> checkPoints);
    ~GameLoop() = default;

    void Run();

    int GetLaps() const { return m_Laps; }
    int GetCheckCount() const { return m_CheckCount; }
    const CheckPointColT& GetCheckPoints() const { return m_CheckPoints; }
    const PodColT& GetPods() const { return m_Pods; }
    const PodColT& GetOpponentPods() const { return m_OpponentPods; }
    const PilotColT& GetPilots() const { return m_Pilots; }
    int GetTurn() const { return m_Turn; }

private:
    void Update();

    const int m_Laps;
    const int m_CheckCount;
    CheckPointColT m_CheckPoints;
    PodColT m_Pods;
    PodColT m_OpponentPods;
    PilotColT m_Pilots;
    int m_Turn = 0; // Before first turn increments to 1
};

class Pilot : protected MathUtils
{
public:
    using VecT = Pod::VecT;
    using CheckPointColT = std::vector<VecT>;
    using PodColT = std::vector<Pod>;

    enum class Action : char
    {
        Thrust, Boost, Shield
    };

    Pilot(const GameLoop& game, size_t index)
    : m_Game(game)
    , m_Pod(game.GetPods()[index])
    , m_OtherPod(GetOtherPod(index))
    , m_Teammate(GetOtherPilot(index))
    {
    }

    virtual ~Pilot() = default;

    virtual void Update()
    {
        m_ThrustTarget = m_Game.GetCheckPoints()[m_Pod.GetCheckPointId()];
    }

    const Action GetAction() const { return m_Action; }
    const VecT& GetThrustTarget() const { return m_ThrustTarget; }
    double GetThrustLevel() const { return m_ThrustLevel; }

protected:
    void SetThrust(const VecT& target, double level) 
    { 
        m_ThrustTarget = target;
        m_ThrustLevel = level; 
    }

    static constexpr double FrictionRate = 0.85;
    static constexpr double MaxThrust = 100;
    static constexpr double MaxSpeed
        = MaxThrust * FrictionRate / (1 - FrictionRate);

    const GameLoop& m_Game;
    const Pod& m_Pod;
    const Pod& m_OtherPod;
    const Pilot& m_Teammate;

private:
    const Pod& GetOtherPod(size_t thisIndex) const
    { 
        return m_Game.GetPods()[1 - thisIndex];
    }

    const Pilot& GetOtherPilot(size_t thisIndex) const 
    { 
        return *m_Game.GetPilots()[1 - thisIndex];
    }

    VecT m_ThrustTarget;
    double m_ThrustLevel = 100;
    Action m_Action = Action::Thrust;
};

std::ostream& operator<<(std::ostream& os, const Pilot& p)
{
    os << std::lround(p.GetThrustTarget()[0]) << " "
        << std::lround(p.GetThrustTarget()[1]) << " ";
    switch(p.GetAction())
    {
    case Pilot::Action::Boost:
        return os << "BOOST";
    case Pilot::Action::Shield:
        return os << "SHIELD";
    case Pilot::Action::Thrust:
        return os << std::lround(p.GetThrustLevel());
    }
}

class GuardPilot : public Pilot
{
public:
    GuardPilot(const GameLoop& game, size_t index)
    : Pilot(game, index)
    {
    }

private:
};

class RacePilot : public Pilot
{
public:
    RacePilot(const GameLoop& game, size_t index)
    : Pilot(game, index)
    {
    }

    void Update() override;

private:
    using CurveFuncT = std::function<VecT(double s)>;

    static CurveFuncT CalcCurveFunc(
        const VecT& pt1, const VecT& pt2, const VecT& pt3);

    static void CalcBkwdTurnFromPt2To1UntilAngleCovered(
        const VecT& pt1, const VecT& pt2, const VecT& pt3, double angle);

    VecT m_InitPos;
};

GameLoop GameLoop::FromStdInput()
{
    int laps;
    std::cin >> laps;
    std::cin.ignore();
    std::cerr << laps << std::endl;

    int checkpointCount;
    std::cin >> checkpointCount;
    std::cin.ignore();
    std::cerr << checkpointCount << std::endl;
    
    CheckPointColT checkPoints(checkpointCount);
    for (int i = 0; i < checkpointCount; i++)
    {
        std::cin >> checkPoints[i];
        std::cin.ignore();
        std::cerr << checkPoints[i] << std::endl;
    }
    return GameLoop(laps, checkPoints);
}

GameLoop::GameLoop(int laps, std::vector<VecT> checkPoints)
: m_Laps(laps)
, m_CheckCount(checkPoints.size())
, m_CheckPoints(std::move(checkPoints))
{
    m_Pilots[0] = std::make_shared<RacePilot>(*this, 0);
    m_Pilots[1] = std::make_shared<GuardPilot>(*this, 1);
}

void GameLoop::Run()
{
    while (true)
    {
        Update();
    }
}

void GameLoop::Update()
{
    ++m_Turn;
    Timer timer("Game turn");
    for (auto& p : m_Pods)
    {
        std::cin >> p;
        std::cin.ignore();
        std::cerr << p.GetAngle() << " " << p.GetCheckPointId() << " "
            << p.GetPos()[0] << " " << p.GetPos()[1] << " "
            << p.GetVel()[0] << " " << p.GetVel()[1] << " " 
            << std::endl;
    }
    for (auto& p : m_OpponentPods)
    {
        std::cin >> p;
        std::cin.ignore();
        std::cerr << p.GetAngle() << " " << p.GetCheckPointId() << " "
            << p.GetPos()[0] << " " << p.GetPos()[1] << " "
            << p.GetVel()[0] << " " << p.GetVel()[1] << " " 
            << std::endl;
    }
    for (auto& p : m_Pilots)
    {
        p->Update();
        std::cerr << *p << std::endl;
    }
    timer.Check();
}

void RacePilot::CalcBkwdTurnFromPt2To1UntilAngleCovered(
    const VecT& pt1, const VecT& pt2, const VecT& pt3, double angle)
{
    auto dir1To2 = (pt2 - pt1).GetNormalized();
    auto dir2To3 = (pt3 - pt2).GetNormalized();
    auto dir1To3 = (pt3 - pt1).GetNormalized();
    auto pt2Vel = MaxSpeed * (dir1To2 + dir2To3).GetNormalized();
    
    auto pos = pt2;
    auto vel = pt2Vel;
    while (vel.AngleTo(pt2Vel) < angle)
    {
        auto velDir = vel.GetNormalized();
        auto vel3D = vel.ChangeDims<3>();
        auto accDir = dir1To2 * vel >= 0 ? dir1To3 : 
            vel3D.Cross(dir1To3.ChangeDims<3>()).
                Cross(vel3D).ChangeDims<2>().GetNormalized();
        auto acc = MaxThrust * accDir;
        vel = vel / FrictionRate - acc;
        pos -= vel;
    }
    
}

void RacePilot::Update()
{
    auto turn = m_Game.GetTurn();
    auto chkPts = m_Game.GetCheckPoints();
    auto numChks = m_Game.GetCheckCount();
    auto chkId = m_Pod.GetCheckPointId();
    auto curChk = chkPts[chkId];
    auto nextChk = chkPts[(chkId + 1) % numChks];
    auto lastChk = chkPts[(chkId - 1) % numChks];
    auto ang = m_Pod.GetAngle();
    auto pos = m_Pod.GetPos();
    auto vel = m_Pod.GetVel();
    
    auto posToCurChkDir = (curChk - pos).GetNormalized();
    auto curToNextChkDir = (nextChk - curChk).GetNormalized();
    auto posToNextChkDir = (nextChk - pos).GetNormalized();
    auto curChkVel = MaxSpeed * (posToCurChkDir + curToNextChkDir).GetNormalized();
    
    auto curPos = curChk;
    auto curVel = curChkVel;
    auto curVelDir = curVel.GetNormalized();
    auto curVel3D = curVel.ChangeDims<3>();
    auto prevAccDir = posToCurChkDir * curVel >= 0 ? posToNextChkDir : 
        curVel3D.Cross(posToNextChkDir.ChangeDims<3>()).
            Cross(curVel3D).ChangeDims<2>();
    auto prevAcc = MaxThrust * prevAccDir;
    auto prevVel = curVel / FrictionRate - prevAcc;
    auto prevPos = curPos - prevVel;


    // auto fouthPt = (nextChk - curChk) / 2;

    // using MatT = Matrix<double, 4, 4>;
    // MatT::ElemsArrayT elemsArr {
    //     { pos[0] * pos[0], pos[1] * pos[1], pos[0], pos[1] },
    //     { curChk[0] * curChk[0], curChk[1] * curChk[1], curChk[0], curChk[1] },
    //     { nextChk[0] * nextChk[0], nextChk[1] * nextChk[1], nextChk[0], nextChk[1] },
    //     { fouthPt[0] * fouthPt[0], fouthPt[1] * fouthPt[1], fouthPt[0], fouthPt[1] }
    // };
    // MatT eqSys{elemsArr};
    // std::cerr << "MatT(elemsArr):" << eqSys << std::endl;
    // MatT eqSolver;
    // if (!eqSys.CalcInverse(eqSolver))
    // {
    //     std::cerr << "Inverse calc failed!" << std::endl;
    // }
    // std::cerr << "Solver:" << eqSolver << std::endl;
    // auto curveCoeffs = eqSolver * Matrix<double, 4, 1>(1);
    // // These four coefficients form an equation that describes the curve that
    // // we should adhere to: a1 * x ^ 2 + a2 * y ^ 2 + a3 * x + a4 * y - 1 = 0
    // std::cerr << curveCoeffs << std::endl;
    // constexpr double maxSpeed = 600;
    // auto startDir = vel.IsNull() ? (curChk - pos) : vel;
    // startDir.Normalize();
    // auto estimTarg = pos + startDir * maxSpeed; // Estimated target point that is supposed to be close to the curve
    // auto crossPathDir = VecT(-startDir[1], startDir[0]);
    // auto crossPathSlope = -startDir[1] / startDir[0];
    // auto crossPathSlopeSqr = crossPathSlope * crossPathSlope;
    // double cc[4] { curveCoeffs(0, 0), curveCoeffs(1, 0), curveCoeffs(2, 0), curveCoeffs(3, 0) };
    // auto crossPointXs = SolveQuadraticEquation(
    //     cc[0] + cc[1] * crossPathSlopeSqr, 
    //     -2 * cc[1] * crossPathSlopeSqr * estimTarg[0] + cc[3] * crossPathSlope + cc[2],
    //     cc[1] * crossPathSlopeSqr * estimTarg[0] * estimTarg[0] - cc[3] * crossPathSlope * estimTarg[0] - 1);
    // auto targetPoint = pos;
    // std::cerr << "crossPointXs: " << crossPointXs.size() << std::endl;
    // for (auto cpx : crossPointXs)
    // {
    //     auto crossPointYs = SolveQuadraticEquation(
    //         cc[1], cc[3], cc[0] * cpx * cpx + cc[2] * cpx - 1);
    //     std::cerr << "crossPointYs: " << crossPointYs.size() << std::endl;
    //     for (auto cpy : crossPointYs)
    //     {
    //         auto crossPoint = VecT(cpx, cpy);
    //         std::cerr << "crossPoint: " << crossPoint << std::endl;
    //         std::cerr << "(crossPoint - estimTarg).GetNorm(): " << (crossPoint - estimTarg).GetNorm() << std::endl;
    //         std::cerr << "(targetPoint - estimTarg).GetNorm(): " << (targetPoint - estimTarg).GetNorm() << std::endl;
    //         if ((crossPoint - estimTarg).GetNorm() < (targetPoint - estimTarg).GetNorm())
    //         {
    //             targetPoint = crossPoint;
    //         }
    //     }
    // }

    auto curveFunc = CalcCurveFunc(pos, curChk, nextChk);
    auto targetDist = 3000;
    constexpr auto sections = 1000;
    auto targetPrecision = (pos - curChk).GetNorm() * 5 / sections;
    auto targetPoint = curChk;
    for (auto i = 0; i < sections; ++i)
    {
        auto s = 4 * i / static_cast<double>(sections);
        auto pt = curveFunc(s);
        if (AreEqualWithEpsilon((pt - pos).GetNorm(), targetDist, targetPrecision))
        {
            std::cerr << "Curve point " << i << ": " << curveFunc(s) << std::endl;
            targetPoint = pt;
            break;
        }
    }

    std::cerr << "Target point: " << targetPoint << std::endl;
    auto neededAcc = (targetPoint - pos - vel * FrictionRate) / FrictionRate;
    auto setThrustTarg = neededAcc.GetNormalized() * (curChk - pos).GetNorm() + pos;
    auto setThrustPower = std::min(neededAcc.GetNorm(), 100.);
    std::cerr << "Needed power: " << neededAcc.GetNorm() << std::endl;
    SetThrust(targetPoint, setThrustPower);
}

RacePilot::CurveFuncT RacePilot::CalcCurveFunc(
    const VecT& pt1, const VecT& pt2, const VecT& pt3)
{
    // The most generic quadratic curve is of the form:
    // x = a * s ^ 2 + b * s + c
    // y = d * s ^ 2 + e * s + f
    // Since the dimensions of the input (s) of the curve function is not specified, 
    // we could define it such that s = 0 corresponds to pt1 (determining c and f) and 
    // s = 1 to pt2 (determining b and e).
    // We shall further stipulate the constraint on the curve that its tangent at pt2
    // be perpendicular to the line bisecting the angle formed around pt2 by pt1-pt2 and pt2-pt3.
    // The missing coefficients are now reduced to only "a":
    // x = a * s ^ 2 + (x2 - x1 - a) * s + x1
    // y = ((a + x2 - x1) * tan + y1 - y2) * s ^ 2 + ((x1 - x2 - a) * tan + 2 * y1 - 2 * y2) * s + y1
    // where x1, y1, x2, and y2 are the coordinates of pt1 and pt2 and a is the remaining coefficient.
    // To find a, we substitute x3 in the x equation and solve for s; we call that s3. 
    // Then we substitute s3 and y3 in the y equation and solve for a.
    // This will require solving a cubic equation with at most three distinct values for a. 
    // We shall pick the one that satisfies the other constraints. These constraints are:
    // - The x equation must be solvable for s given x1, x2, or x3 for x.
    // - The y equation must be solvable for s given y1, y2, or y3 for y.
    // - s3 (corresponding to pt3) must be greater than 1
    // The last constraint is to ensure that the curve crosses the three points in the desired order.
    auto tanAtPt2 = ((pt2 - pt1).GetNormalized() 
        + (pt3 - pt2).GetNormalized()).GetNormalizedOrZero();
    if (IsZeroWithEpsilon(tanAtPt2[0]))
    {
        tanAtPt2[0] = .01;
        tanAtPt2.Normalize();
    }

    auto x1 = pt1[0], x2 = pt2[0], x3 = pt3[0];
    auto y1 = pt1[1], y2 = pt2[1], y3 = pt3[1];

    auto b = x3 - x2;
    auto c = x1 - x2;
    auto d = 2 * (2 * x3 - x1 - x2);
    auto e = y1 - y2;
    auto f = tanAtPt2[1] / tanAtPt2[0];
    auto h = -c * f + e;
    auto i = c * f - 2 * e;

    auto k = y1 - y3 + 2 * b * f - c * f - e;
    auto l = c * c + 2 * b * h + c * i;
    auto m = c * c * h;
    auto n = c * f + h + i;
    auto o = c * h;

    auto p = k * k - n * n;
    auto q = 2 * k * l - d * n * n - 2 * n * o;
    auto r = l * l + 2 * k * m - c * c * n * n - o * o - 2 * d * n * o;
    auto t = 2 * l * m - d * o * o;

    std::function<double(double)> eqFunc = [p, q, r, t](auto a) { 
        return p * a * a * a + q * a * a + r * a + t; };

    double bestA = 0;
    double minS = 1000;
    for (auto a : FindEquationRoots(eqFunc, 0., 10000., 3))
    {
        auto s3candidates = SolveQuadraticEquation(a, x2 - x1 - a, x1 - x3);
        if (s3candidates.empty())
        {
            continue;
        }

        for (auto s3 : s3candidates)
        {
            auto b = ((a + x2 - x1) * f + y1 - y2);
            auto yForS3 = b * s3 * s3 + (y2 - y1 - b) * s3 + y1;
            if (AreEqualWithEpsilon(y3, yForS3) && s3 < minS)
            {
                minS = s3;
                bestA = a;
            }
        }
    }

    auto a1 = bestA;
    auto a2 = -(bestA + c);
    auto a3 = (bestA + c) * f + e;
    auto a4 = -(bestA + c) * f - 2 * e;

    auto curveFunc = [a1, a2, a3, a4, x1, y1](double s)
    {
        return VecT(a1 * s * s + a2 * s + x1, a3 * s * s + a4 * s + y1);
    };
    
    return curveFunc;
}


} // namespace csb

int main()
{
    auto timer = csb::Timer("Init sequence");
    auto game = csb::GameLoop::FromStdInput();
    timer.Check();
    game.Run();
}
